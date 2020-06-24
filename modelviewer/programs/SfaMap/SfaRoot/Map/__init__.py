import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import os
import os.path
from common.sfa.ModelLoader import ModelLoader
from common.gamecube.DlistParser import DlistParser
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from .MapObject import MapObject

P = os.path.join

mapTable = {}
def M(id, name, dir):
    entry = {
        'id':   id,
        'name': name,
        'dir':  dir,
    }
    mapTable[id] = entry
    mapTable[name] = entry

M(0x00,'frontend'       ,'shipbattle')
M(0x01,'frontend2'      ,'animtest')
M(0x02,'dragrock'       ,'dragrock')
M(0x03,'krazoapalace'   ,'animtest')
M(0x04,'temple'         ,'volcano')
M(0x05,'hightop'        ,'animtest')
M(0x06,'discovery'      ,'animtest')
M(0x07,'hollow'         ,'swaphol')
M(0x08,'hollow2'        ,'swapholbot')
M(0x09,'mazecave'       ,'mazecave')
M(0x0a,'wastes'         ,'nwastes')
M(0x0b,'warlock'        ,'warlock')
M(0x0c,'fortress'       ,'crfort')
M(0x0d,'wallcity'       ,'wallcity')
M(0x0e,'swapcircle'     ,'lightfoot')
M(0x0f,'cloudtreasure'  ,'cloudtreasure')
M(0x10,'clouddungeon'   ,'clouddungeon')
M(0x11,'cloudtrap'      ,'animtest')
M(0x12,'moonpass'       ,'mmpass')
M(0x13,'snowmines'      ,'darkicemines')
M(0x14,'krashrin2'      ,'animtest')
M(0x15,'kraztest'       ,'desert')
M(0x16,'krazchamber'    ,'animtest')
M(0x17,'newicemount'    ,'icemountain')
M(0x18,'newicemount2'   ,'animtest')
M(0x19,'newicemount3'   ,'animtest')
M(0x1a,'animtest'       ,'animtest')
M(0x1b,'snowmines2'     ,'darkicemines2')
M(0x1c,'snowmines3'     ,'bossgaldon')
M(0x1d,'capeclaw'       ,'capeclaw')
M(0x1e,'insidegal'      ,'insidegal')
M(0x1f,'dfshrine'       ,'dfshrine')
M(0x20,'mmshrine'       ,'mmshrine')
M(0x21,'ecshrine'       ,'ecshrine')
M(0x22,'gpshrine'       ,'gpshrine')
M(0x23,'diamondbay'     ,'dbay')
M(0x24,'earthwalker'    ,'animtest')
M(0x25,'willow'         ,'animtest')
M(0x26,'arwing'         ,'arwing')
M(0x27,'dbshrine'       ,'dbshrine')
M(0x28,'nwshrine'       ,'nwshrine')
M(0x29,'ccshrine'       ,'worldmap')
M(0x2a,'wgshrine'       ,'animtest')
M(0x2b,'cloudrace'      ,'cloudrace')
M(0x2c,'finalboss'      ,'bossdrakor')
M(0x2d,'wminsert'       ,'animtest')
M(0x2e,'snowmines4'     ,'animtest')
M(0x2f,'snowmines5'     ,'animtest')
M(0x30,'trexboss'       ,'bosstrex')
M(0x31,'mikelava'       ,'animtest')
M(0x32,'dfptop'         ,'dfptop')
M(0x33,'swapstore'      ,'shop')
M(0x34,'dragbot'        ,'dragrockbot')
M(0x35,'kamdrag'        ,'animtest')
M(0x36,'magicave'       ,'magiccave')
M(0x37,'duster'         ,'cloudjoin')
M(0x38,'linkb'          ,'linkb')
M(0x39,'cloudjoin'      ,'animtest')
M(0x3a,'arwingtoplanet' ,'arwingtoplanet')
M(0x3b,'arwingdarkice'  ,'arwingdarkice')
M(0x3c,'arwingcloud'    ,'arwingcloud')
M(0x3d,'arwingcity'     ,'arwingcity')
M(0x3e,'arwingdragon'   ,'arwingdragon')
M(0x3f,'gamefront'      ,'gamefront')
M(0x40,'linklevel'      ,'linklevel')
M(0x41,'greatfox'       ,'greatfox')
M(0x42,'linka'          ,'linka')
M(0x43,'linkc'          ,'linkc')
M(0x44,'linkd'          ,'linkd')
M(0x45,'linke'          ,'linke')
M(0x46,'linkf'          ,'linkf')
M(0x47,'linkg'          ,'linkg')
M(0x48,'linkh'          ,'linkh')
M(0x49,'linkj'          ,'linkj')
M(0x4a,'linki'          ,'linki')


class Map:
    """SFA map."""
    def __init__(self, game):
        self.game      = game
        self.objects   = []
        self.modelInd  = {} # ID => real ID (MODELIND.bin)
        self.modelsTab = {} # ID => offset

    def load(self, nameOrId):
        """Load map by name or ID."""
        entry = mapTable[nameOrId]
        self.dir = P(self.game.discRoot, entry['dir'])
        self.loadModelInd()
        self.loadModelsTab()
        self.loadRomList(entry['name']+'.romlist.zlb')
        return self

    def loadRomList(self, path):
        """Load romlist.zlb file."""
        # these are in disc root dir
        file = BinaryFile(P(self.game.discRoot, path), 'rb')
        data = Zlb(file).decompress()
        offs = 0
        while offs < len(data):
            typ, length = struct.unpack_from('>hB', data, offs)
            if length == 0: break
            entry = data[offs:offs+(length*4)]
            #log.debug("Load object %d", len(self.objects))
            obj   = MapObject(self, entry)
            self.game.loadModel(obj.modelId)
            self.objects.append(obj)
            offs += length * 4

    def loadModelInd(self):
        """Load MODLEIND.bin for this map."""
        self.modelInd = {}
        with BinaryFile(P(self.dir, 'MODELIND.bin'), 'rb') as file:
            idx = 0
            while True:
                try: newIdx = file.readu16()
                except struct.error: break
                if newIdx != 0xFFFF: self.modelInd[idx] = newIdx
                idx += 1

    def loadModelsTab(self):
        """Load MODELS.tab for this map."""
        self.modelsTab = {}
        with BinaryFile(P(self.dir, 'MODELS.tab'), 'rb') as file:
            idx = 0
            while True:
                try: offs = file.readu32()
                except struct.error: break
                if offs == 0xFFFFFFFF: break
                self.modelsTab[idx] = offs
                idx += 1

    def render(self):
        for renderer in self.game.modelRenderers.values():
            renderer.render()

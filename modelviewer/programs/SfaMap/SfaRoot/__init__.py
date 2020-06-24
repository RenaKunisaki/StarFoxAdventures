import logging; log = logging.getLogger(__name__)
import struct
import math
import sys
import os
import os.path
import gl
import numpy as np
from common.sfa.ModelLoader import ModelLoader
from common.sfa.ObjectStruct import ObjectStruct
from common.gamecube.DlistParser import DlistParser
from common.BinaryFile import BinaryFile
from common.sfa.zlb import Zlb
from .ModelRenderer import ModelRenderer
from .Map import Map

P = os.path.join


class SfaRoot:
    """Represents the SFA game disc."""

    def __init__(self, program, discRoot='files/'):
        self.program  = program
        self.discRoot = discRoot
        self.map = None
        self.models = {} # ID => Model
        self.modelRenderers = {}

    def loadGlobalAssets(self):
        """Load files/assets that are shared by all maps."""
        self.loadObjIndex()
        self.loadObjectsTab()
        self.loadObjectsBin()

    def loadObjIndex(self):
        """Load OBJINDEX.bin"""
        self.objIndex = []
        with open(self.discRoot+'/OBJINDEX.bin', 'rb') as objIdxFile:
            entries = objIdxFile.read()
            for i in range(0, len(entries), 2):
                it = struct.unpack_from('>H', entries, i)[0] # grumble
                self.objIndex.append(it)

    def loadObjectsTab(self):
        """Load OBJECTS.tab"""
        self.objsTab = []
        with open(self.discRoot+'/OBJECTS.tab', 'rb') as objTabFile:
            entries = objTabFile.read()
            for i in range(0, len(entries), 4):
                it = struct.unpack_from('>I', entries, i)[0] # grumble
                self.objsTab.append(it)

    def loadObjectsBin(self):
        """Load OBJECTS.bin"""
        self.objDefs = {}
        with open(self.discRoot+'/OBJECTS.bin', 'rb') as objBinFile:
            idx = 0
            for offs in self.objsTab:
                if offs == 0xFFFFFFFF: break
                try:
                    objBinFile.seek(offs)
                    entry = ObjectStruct.readFromFile(objBinFile)
                    self.objDefs[idx] = entry
                except struct.error:
                    log.exception("Error reading objdef 0x%X at 0x%X",
                        idx, offs)
                idx += 1
        log.debug("Loaded %d objdefs", len(self.objDefs))


    def loadModel(self, id):
        """Load model by ID."""
        if id >= 0:
            if id in self.map.modelInd: id = self.map.modelInd[id]
            else: log.warning("Model ID 0x%X not in MODELIND.bin", id)
        else: id = -id
        if id not in self.models:
            path = P(self.map.dir, 'MODELS.bin')
            offs = self.map.modelsTab[id] & 0x00FFFFFF
            with BinaryFile(path, 'rb', offset=offs) as file:
                #log.debug("model 0x%04X offs 0x%08X fsize 0x%08X",
                #    id, offs, file.size)
                try:
                    model = ModelLoader()
                    model.loadFromFile(file)
                except struct.error:
                    log.exception("Failed loading model 0x%X from offset 0x%08X", id, offs)
                    return None
            self.models[id] = model
        return self.models[id]

    def loadMap(self, name):
        """Load given map."""
        log.info("Loading map: %s", name)
        self.map = Map(self)
        self.map.load(name)
        for i, obj in enumerate(self.map.objects):
            s = self.objDefs[obj.type].scale
            pointA = (obj.pos.x - s, obj.pos.y - s, obj.pos.z - s)
            pointB = (obj.pos.x + s, obj.pos.y + s, obj.pos.z + s)
            T = obj.type
            color  = (
                (( T       & 3) | (((T >>  6) & 7) << 2)) / 31,
                (((T >> 2) & 3) | (((T >>  9) & 7) << 2)) / 31,
                (((T >> 4) & 3) | (((T >> 12) & 7) << 2)) / 31,
                0.5)
            #self.program.boxRenderer.addBox(pointA, pointB, color)

            try: self.loadModel(obj.modelId)
            except:
                log.exception("Error loading model 0x%X", obj.modelId)
            try: model = self.models[obj.modelId]
            except KeyError:
                log.warning("Model 0x%X not loaded", obj.modelId)
                model = self.models[0]
            if obj.modelId not in self.modelRenderers:
                renderer = ModelRenderer(self.program)
                renderer.setModel(model)
                self.modelRenderers[obj.modelId] = renderer
            obj.render()

    def setMtxs(self, projection, modelView):
        """Set render matrices."""
        for renderer in self.modelRenderers.values():
            renderer.dlistRenderer.setMtxs(projection, modelView)

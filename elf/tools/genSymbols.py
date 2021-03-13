#!/usr/bin/env python3
"""Generate header and symbol map files from XML."""
import xml.etree.ElementTree as ET
import textwrap
import sys
import os.path

outHeader = None # will set later
outMap    = {}


def str2bool(s):
    return s.lower() == 'true'

def attr2bool(elem, name, dflt=None):
    """Get attribute `name` from XML Element `elem`. If it's present,
    convert it to `bool` and return it; otherwise return `dflt`.
    """
    try: v = elem.get(name)
    except KeyError: return dflt
    if v is None: return dflt
    return str2bool(v)

def attr2int(elem, name, dflt=None):
    """Get attribute `name` from XML Element `elem`. If it's present,
    convert it to `int` and return it; otherwise return `dflt`.
    """
    try: v = elem.get(name)
    except KeyError: return dflt
    if v is None: return dflt
    return int(v, 0)

def getAttributes(elem):
    """Get text of all `attribute` tags under `attributes` tag.
    Return list of texts.
    """
    attrs = set()
    eAttrs = elem.find('attributes')
    if eAttrs is not None:
        for eAttr in eAttrs.findall('attribute'):
            attrs.add(eAttr.text)
    return attrs

def getParam(elem, arg=None):
    """Get attributes and children of a parameter or return type element.
    Populate an existing dict or create a new one, and return it.
    """
    if arg is None: arg = {}
    arg['type']        = elem.get('type')
    arg['count']       = elem.get('count', None)
    arg['name']        = elem.get('name',  None)
    arg['dir']         = elem.get('dir',   None)
    arg['min']         = elem.get('min',   None)
    arg['max']         = elem.get('max',   None)
    arg['align']       = elem.get('align', None)
    arg['allowNull']   = attr2bool(elem, 'allowNull')
    arg['attributes']  = getAttributes(elem)
    #if arg['allowNull'] == False: arg['attributes'].add('XXX') # does gcc have nonnull for args?
    if attr2bool(elem, 'unused') == True: arg['attributes'].add('unused')

    eDescr = elem.find('description')
    if eDescr is None: arg['description'] = None
    else: arg['description'] = eDescr.text

    return arg

def formatParam(param):
    """Build C code string for a function parameter."""
    # note, 'align' does NOT mean the same as gcc's 'align' attribute.
    # the latter has to do with memory alignment.
    # the former tells the alignment of valid values; eg "align 32" means
    # that the value must be a multiple of 32.
    name = param['name']
    if name is None: name = ''
    if param['count'] is not None: name += '[%s]' % param['count']
    res = [param['type'], name]
    for attr in param['attributes']: res.append(' __attribute__ ((%s))' % attr)
    return ' '.join(res)

def formatCommentLine(text):
    """Nicely format a line for a block comment."""
    return textwrap.wrap(textwrap.dedent(text.replace('\n', ' ')))

def makeParamComment(param):
    """Build the block comment lines for a function parameter."""
    descr = param['description']
    if descr is None: descr = ''
    if param['allowNull']: descr = '[allowNull] ' + descr
    if param['dir'] is not None: descr = '[%s] %s' % (param['dir'], descr)
    details = []
    if param['min']   is not None: details.append('min:'  +param['min'])
    if param['max']   is not None: details.append('max:'  +param['max'])
    if param['align'] is not None: details.append('align:'+param['align'])
    if len(details) > 0:
        descr += ' (%s)' % (', '.join(details))
    descr = descr.strip()
    if len(descr) == 0: return ''
    return formatCommentLine('@param %s %s' % (param['name'], descr))

def makeReturnComment(ret):
    """Build the block comment lines for a function return value."""
    descr = ret['description']
    if descr is None: descr = ''
    if ret['allowNull']: descr = '[allowNull] ' + descr
    details = []
    if ret['min']   is not None: details.append('min:'  +ret['min'])
    if ret['max']   is not None: details.append('max:'  +ret['max'])
    if ret['align'] is not None: details.append('align:'+ret['align'])
    if len(details) > 0:
        descr += ' (%s)' % (', '.join(details))
    descr = descr.strip()
    if len(descr) == 0: return ''
    return formatCommentLine('@return ' + descr)

def makeComment(fields):
    """Build the block comment for a symbol."""
    comment = []
    descr = fields.get('description', None)
    if descr is not None: comment += formatCommentLine(descr.text)

    params = fields.get('params', None)
    if params is None: params = [] # in case None was explicitly passed
    for param in params: comment += makeParamComment(param)

    retVal = fields.get('retVal', None)
    if retVal is not None: comment += makeReturnComment(retVal)

    notes = fields.get('notes', None)
    if notes is None: notes = []
    for note in notes:
        comment += formatCommentLine('@note '+note.text)

    res = []
    if len(comment) > 0:
        res.append('/**\n')
        for i, line in enumerate(comment):
            res.append('  * '+line+'\n')
        res.append('  */\n')
    return ''.join(res)

def writeFunc(eSym):
    """Write the C code and comment for a function declaration."""
    name   = eSym.get('name')
    retVal = {
        'type':       'void',
        'allowNull':   None,
        'description': None,
        'min':         None,
        'max':         None,
        'align':       None,
        'count':       None,
    }
    eRet = eSym.find('return')
    if eRet is not None: getParam(eRet, retVal)

    # get params
    params   = []
    isVararg = False
    eParams  = eSym.find('params')
    if eParams is not None:
        isVararg = attr2bool(eParams, 'vararg')
        for eParam in eParams.findall('param'):
            params.append(getParam(eParam))

    # get function attributes
    funcAttrs = getAttributes(eSym)
    if attr2bool(eSym, 'noreturn'): funcAttrs.add('noreturn')
    if retVal['allowNull'] == False: funcAttrs.add('returns_nonnull')
    eAttrs = eSym.find('attributes')
    if eAttrs is not None:
        for eAttr in eAttrs.findall('attribute'):
            funcAttrs.add(eAttr.text)

    # write comment
    outHeader.write(makeComment({
        'description': eSym.find('description'),
        'params': params,
        'retVal': retVal,
        'notes':  eSym.findall('note'),
    }))

    # write signature
    retTyp = retVal['type']
    if retVal['count'] is not None: retTyp += '[%s]' % retVal['count']
    outHeader.write('%s %s(' % (retTyp, name))
    paramStrs = [formatParam(p) for p in params]
    if isVararg: paramStrs.append('...')
    outHeader.write(', '.join(paramStrs))
    outHeader.write(')')
    for attr in funcAttrs: outHeader.write(' __attribute__ (( %s ))' % attr)
    outHeader.write(';\n')


def writeFuncdef(filePath, outFile, eType):
    """Write the C code for a function typedef."""
    name   = eType.get('name')
    retVal = {
        'type':       'void',
        'allowNull':   None,
        'description': None,
        'min':         None,
        'max':         None,
        'align':       None,
        'count':       None,
    }
    eRet = eType.find('return')
    if eRet is not None: getParam(eRet, retVal)

    # get params
    params   = []
    isVararg = False
    eParams  = eType.find('params')
    if eParams is not None:
        isVararg = attr2bool(eParams, 'vararg')
        for i, eParam in enumerate(eParams.findall('param')):
            param = getParam(eParam)
            if param['name'] is None or param['name'] == '':
                param['name'] = 'param%d' % (i+1)
            params.append(param)

    # write signature
    retTyp = retVal['type']
    if retVal['count'] is not None: retTyp += '[%s]' % retVal['count']
    outFile.write('typedef %s (*%s)(' % (retTyp, name))
    paramStrs = [formatParam(p) for p in params]
    if isVararg: paramStrs.append('...')
    outFile.write(', '.join(paramStrs))
    outFile.write(');\n')

def writeVar(eSym):
    """Write the C code and comment for a variable declaration."""
    name  = eSym.get('name')
    typ   = eSym.get('type')
    attrs = getAttributes(eSym)

    # write comment
    outHeader.write(makeComment({
        'description': eSym.find('description'),
        'notes':  eSym.findall('note'),
    }))

    # write signature
    count = eSym.get('count', None)
    if count is not None: name += '[%s]' % count
    outHeader.write('extern %s %s' % (typ, name))
    for attr in attrs: outHeader.write(' __attribute__ (( %s ))' % attr)
    outHeader.write(';\n')


def writeSymbols(filePath, outPath):
    """Write the symbol definition/declaration files."""
    global outHeader
    global outMap
    outHeader = open(os.path.join(outPath, 'symbols-generated.h'), 'wt')
    outMap    = {
        'U0': open(os.path.join(outPath, 'symbols-generated.sym'), 'wt'),
    }
    outHeader.write("/** Auto generated by genSymbols.py */\n")


    # parse symbols.xml and generate the header and symbol map files.
    syms = ET.parse(os.path.join(filePath, 'symbols.xml')).getroot()
    for eSym in syms.findall('symbol'):
        typ  = eSym.get('type')
        if typ == 'function': writeFunc(eSym)
        else: writeVar(eSym)
        name = eSym.get('name')
        for eAddr in eSym.findall('address'):
            ver = eAddr.get('version')
            if ver in outMap:
                outMap[ver].write('%s = %s;\n' % (name, eAddr.text))


allStructsAndUnions = {}

def writeDefine(filePath, outFile, eType):
    """Write the C code for a #define directive."""
    val = eType.find('value')
    if val is not None: val = val.text
    else: val = '' # empty defines are valid
    outFile.write('//%s\n' % filePath)
    outFile.write('#define %s %s\n' % (eType.get('name'), val))
    # XXX description


def writeEnum(filePath, outFile, eType):
    """Write the C code for an enum."""
    prefix = eType.get('prefix', '')
    # XXX description
    #allStructsAndUnions[eType.get('name')] = 'enum'
    gotNames = {}
    outFile.write('//%s\n' % filePath)
    outFile.write('typedef enum { //type:%s\n' % eType.get('type'))
    for eMember in eType.findall('member'):
        mPrefix = eMember.get('prefix', prefix)
        name = mPrefix + eMember.get('name')
        val  = eMember.get('value')
        if name in gotNames:
            if gotNames[name] == val: continue
            else:
                print("Duplicate enum entry %s.%s (%s, %s)" % (
                    eType.get('name'), name, val, gotNames[name]))
                continue
            gotNames[name] = val
        outFile.write('\t%s = %s,\n' % (name, val))
    outFile.write('} %s;\n' % eType.get('name'))


def writeTypedef(filePath, outFile, eType):
    """Write the C code for a typedef."""
    # XXX description
    outFile.write('//%s\n' % filePath)
    outFile.write('typedef %s %s;\n' % (eType.text, eType.get('name')))


def writeStruct(filePath, outFile, eType):
    """Write the C code for a struct definition."""
    # XXX description

    allStructsAndUnions[eType.get('name')] = 'union' # see below

    # horrible hack to be able to place things at particular offsets by wrapping
    # every field in a union with some padding in front.
    outFile.write('//%s\n' % filePath)
    outFile.write('typedef union %s {\n' % eType.get('name'))

    for eField in eType.findall('field'):
        name = eField.get('name')
        if int(eField.get('offset'), 0) == 0:
            outFile.write('\t%s %s;\n' % (eField.get('type'), name))
        else:
            outFile.write('\tstruct __attribute__((packed)) { u8 _pad_%s[%s]; %s %s; };\n' % (
                name, eField.get('offset'), eField.get('type'), name,
            ))

    outFile.write('} %s;\n' % eType.get('name'))


def writeUnion(filePath, outFile, eType):
    """Write the C code for a union definition."""
    # XXX description

    allStructsAndUnions[eType.get('name')] = 'union'

    outFile.write('//%s\n' % filePath)
    outFile.write('typedef union %s {\n' % eType.get('name'))

    for eField in eType.findall('field'):
        name = eField.get('name')
        outFile.write('\t%s %s;\n' % (eField.get('type'), name))

    outFile.write('} %s;\n' % eType.get('name'))


processedFiles = set()
def writeTypes(filePath, outFile, _depth=0):
    """Write the type definition files."""
    assert _depth < 5, "Maximum include depth exceeded"
    if filePath in processedFiles:
        print("Duplicate include:", filePath)
        return
    else: processedFiles.add(filePath)
    #print("processing", filePath)

    defines  = []
    enums    = []
    structs  = []
    funcdefs = []
    typedefs = []
    unions   = []
    eTypes   = ET.parse(filePath).getroot()
    for eType in eTypes:
        if eType.tag == 'include':
            tPath = eType.get('path') # tag (include) path
            sPath = tPath.replace('..', '') # sanitized (output) path
            while sPath.startswith('/'): sPath = sPath[1:]
            sPath = os.path.join(os.path.dirname(outFile.name), sPath)
            sDir  = os.path.dirname(sPath)
            sName = os.path.basename(sPath)
            if     sName.lower().endswith('.xml'): sName = sName[0:-4]
            sName = sName.replace('.h', '')
            #if not sName.lower().endswith('.h'):   sName += '.h'
            sPath = os.path.join(sDir, sName)
            #print("mkdir", sPath)
            os.makedirs(sPath, exist_ok=True)
            #sPath = os.path.join(sPath, 'types.h')
            sPath = os.path.join(sPath, sName + '.h')
            #print("write", sPath)
            # XXX fix this shit
            outFile.write('#include "../%s"\n' % sPath)
            try:
                with open(sPath, 'at') as newOutFile:
                    writeTypes(os.path.join(os.path.dirname(filePath), tPath),
                        newOutFile, _depth=_depth+1)
            except Exception as ex:
                sys.stderr.write("Error writing %s: %r\n" % (sPath, ex))
                raise
        elif eType.tag == 'define':  defines .append((filePath, outFile, eType))
        elif eType.tag == 'enum':    enums   .append((filePath, outFile, eType))
        elif eType.tag == 'struct':  structs .append((filePath, outFile, eType))
        elif eType.tag == 'funcdef': funcdefs.append((filePath, outFile, eType))
        elif eType.tag == 'typedef': typedefs.append((filePath, outFile, eType))
        elif eType.tag == 'union':   unions  .append((filePath, outFile, eType))
        else: raise ValueError("Unrecognized tag '%s' in '%s'" % (
            eType.tag, filePath))

    for item in defines:  writeDefine (*item)
    for item in enums:    writeEnum   (*item)
    for item in typedefs: writeTypedef(*item)
    for item in funcdefs: writeFuncdef(*item)
    for item in structs:  writeStruct (*item)
    for item in unions:   writeUnion  (*item)



if len(sys.argv) < 3:
    print("Usage: genSymbols.py inDir outDir")
    sys.exit(1)
filePath = sys.argv[1]
outPath  = sys.argv[2]
writeSymbols(filePath, outPath)
with open(os.path.join(outPath,'types-generated.h'), 'wt') as outFile:
    outFile.write('#include "typedefs.h"\n')
    writeTypes(os.path.join(filePath, 'types.xml'), outFile)

# typedef all the things so that we don't need to worry about order as much.
with open(os.path.join(outPath, 'typedefs.h'), 'wt') as outFile:
    for name, typ in allStructsAndUnions.items():
        outFile.write('typedef %s %s %s;\n' % (typ, name, name))

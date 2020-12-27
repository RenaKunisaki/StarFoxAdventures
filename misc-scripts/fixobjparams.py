#!/usr/bin/env python3
"""Changes format of object parameter list."""
import sys
import os
import os.path
import xml.etree.ElementTree as ET

objs = ET.parse(sys.argv[1]).getroot()
for obj in objs.findall("./object"):
    params = obj.find("./params")
    if params:
        newParams = []
        for param in params:
            newParam = {'type': param.tag}
            descr = "".join(param.itertext())
            for k, v in param.items():
                newParam[k] = v
            newParams.append({
                'attrs': newParam,
                'descr': descr.strip(),
            })
        params.clear()
        for param in newParams:
            elem = ET.SubElement(params, 'param', param['attrs'])
            if param['descr'] != '':
                ET.SubElement(elem, 'description').text = param['descr']

print(ET.tostring(objs, encoding='unicode'))

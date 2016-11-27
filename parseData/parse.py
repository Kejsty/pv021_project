import xml.etree.ElementTree

e = xml.etree.ElementTree.parse('data/a01/a01-001/strokesz.xml').getroot()

outFile = open('output', 'w')
#
# for set in e.iter('Stroke'):
#     for child in set.iter('Point'):
#         (child.tag, child.attrib)


outputList = ""


for set in e.iter('Stroke'):
    for child in set.iter('Point'):
        lastPointX = 0
        lastPointy = 0
        outputList += "\n" + child.get('x') + " " + child.get('y') +  ' 0'
    outputList += "1"
    outFile.write(outputList)
    outputList = ""



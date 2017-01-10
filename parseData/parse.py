import xml.etree.ElementTree

e = xml.etree.ElementTree.parse(r'strokesz007.xml').getroot()

outFile = open('output_007', 'w')


outputList = ""

offsets = []
last_x = 0
last_y = 0
maximum = 0
for set in e.iter('Stroke'):
    for child in set.iter('Point'):
        x = int(child.get('x'))
        y = int(child.get('y'))
        off_x = x - last_x
        off_y = y - last_y
        offsets.append((off_x, off_y, 0))
        last_x = x
        last_y = y
        maximum = max(maximum, abs(off_x), abs(off_y))
    offsets[-1] = (offsets[-1][0],offsets[-1][1],1)
offsets[0] = (0,0,0)

for rec in offsets:
    if(rec[0]/maximum > -0.5):
        outputList += str(rec[0]/maximum) + " " + str(rec[1]/maximum) + " " + str(rec[2]) + "\n"
    else: #end of line, start new sequence
        outputList += "0 0 0\n"
        outputList += "0 0 0\n"
outFile.write(outputList)






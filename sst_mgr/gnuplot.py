key_value = 52
filename = 'data/2/pccsbox6_top3_6291456.dat'

with open(filename) as f:
    content = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
content = [x.strip('\n') for x in content]
content = content[1:len(content)]
items = []
items_greater = []
pcc_value = 0
for x in content:
    items.append(x.split('        '))

for item in items:
    if item[0] == str(key_value):
        pcc_value = item[1]

print pcc_value
for item in items:
    if item[1] > pcc_value:
        items_greater.append(item)

print len(items_greater)
for item in items_greater:
    print item

f = open("input.txt", "r")

s = set()

final = []
unique = 0;
for line in f:
  line.split(',')
  res = [s.strip() for s in line.split(',')]
  final.append(res)

  host = res[1]

  unique += int(res[2])
  # if(int(res[2]) == 0):
  #   print(f"My duplicate: {res[1]}")

  # if(host in s):
  #   print(f"[{res[2]}] Duplicate host {host}")
  
  if(host not in s):
    print(f"[{res[2]}] NO Duplicate host {host}")
  

  else:
    s.add(res[1]) # add host

# print(final)
print(f"Length of final: {len(final)}")
print(f"Number of unique host: {unique}")
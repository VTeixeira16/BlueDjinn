import os, shutil

cwd = os.getcwd()

print('cwd: {0}'.format(cwd))
print('Clean Build')

for root, dirs, files in os.walk(cwd):
    for f in files:
        os.unlink(os.path.join(root, f))
    for d in dirs:
        print(f'd: {d}')
        if(os.path.isdir(d)):
            if os.path.isdir(d):
                 if os.path.islink(d):
                      os.unlink(d)
                 else:
                     shutil.rmtree(d)
            else:
                 if os.path.islink(d):
                     os.unlink(d)
                 else:
                     os.remove(d)

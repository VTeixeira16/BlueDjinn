import os, shutil

cwd = os.getcwd()

print('cwd: {0}'.format(cwd))
print('Clean Build')

for root, dirs, files in os.walk(cwd):
    for f in files:
        os.unlink(os.path.join(root, f))
    for d in dirs:
        shutil.rmtree(os.path.join(root, d))

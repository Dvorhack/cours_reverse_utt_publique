import glob
from pathlib import Path


challenges = {}

for x in glob.glob("exercices/*/*"):

    chall = Path(f"{x}/writeup")
    chall_name = x.split('/')[-1]
    
    if chall.is_dir() and Path(f"{x}/writeup/writeup.md").is_file():
        print(chall_name, "done")
        challenges[x] = True
    else:
        print(chall_name, "TODO")
        challenges[x] = False


template = f"""
# cours_reverse_utt_publique

## Writeups

| Challenge Name  | Have Writeup |
| ------------- | ------------- |
"""

for name, done in challenges.items():
    _, cat, chall_name = name.split('/')
    if done:
        template += f"| {cat} -> [{chall_name}]({name})  |  ✅  |\n"
    else:
        template += f"| {cat} -> [{chall_name}]({name})  |  ❌  |\n"

with open("README.md", 'w') as f:
    f.write(template)
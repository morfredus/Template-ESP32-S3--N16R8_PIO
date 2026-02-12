Import("env")
import os, hashlib, json

HASH_FILE = ".pio/littlefs_hash.json"
DATA_DIR = "data"

def hash_directory(path):
    sha = hashlib.sha256()
    for root, dirs, files in os.walk(path):
        for f in sorted(files):
            full = os.path.join(root, f)
            sha.update(full.encode())
            with open(full, "rb") as fp:
                sha.update(fp.read())
    return sha.hexdigest()

def before_upload(source, target, env):
    print("=== Vérification du FS LittleFS ===")

    if not os.path.isdir(DATA_DIR):
        print("Pas de dossier data/, skip FS")
        return

    new_hash = hash_directory(DATA_DIR)
    old_hash = None

    if os.path.isfile(HASH_FILE):
        try:
            with open(HASH_FILE, "r") as fp:
                old_hash = json.load(fp).get("hash")
        except:
            pass

    if new_hash == old_hash:
        print("FS inchangé → pas d'uploadfs")
        return

    print("FS modifié → buildfs + uploadfs")
    env.Execute("pio run --target buildfs")
    env.Execute("pio run --target uploadfs")

    os.makedirs(os.path.dirname(HASH_FILE), exist_ok=True)
    with open(HASH_FILE, "w") as fp:
        json.dump({"hash": new_hash}, fp)

env.AddPreAction("upload", before_upload)

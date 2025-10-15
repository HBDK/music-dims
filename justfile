reset-to-origin branch:
  GIT_SSH_COMMAND='ssh -i ~/.ssh/proxykey -o IdentitiesOnly=yes' git fetch origin
  git reset --hard origin/{{branch}}
  ./update-git-commit.sh

pull:
  GIT_SSH_COMMAND='ssh -i ~/.ssh/proxykey -o IdentitiesOnly=yes' git pull
  ./update-git-commit.sh

decrypt:
  sops decrypt --input-type json --output-type json ./backend/MusicDimsApi/Api/secrets.json.enc > ./backend/MusicDimsApi/Api/appsettings.secrets.json

encrypt:
  sops --encrypt ./backend/MusicDimsApi/Api/appsettings.secrets.json > ./backend/MusicDimsApi/Api/secrets.json.enc

build-and-deploy:
  docker compose up -d --build

deploy: pull decrypt build-and-deploy

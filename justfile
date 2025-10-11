reset-to-origin branch:
  GIT_SSH_COMMAND='ssh -i ~/.ssh/proxykey -o IdentitiesOnly=yes' git fetch origin
  git reset --hard origin/{{branch}}
  ./update-git-commit.sh

pull:
  GIT_SSH_COMMAND='ssh -i ~/.ssh/proxykey -o IdentitiesOnly=yes' git pull
  ./update-git-commit.sh

decrypt:
  sops decrypt --input-type json --output-type json src/WeedProxy/secrets.json.enc > src/WeedProxy/appsettings.secrets.json

build-and-deploy:
  docker compose up -d --build

follow-logs:
  docker logs weedproxy-weedproxy-1 -f

deploy: pull decrypt build-and-deploy

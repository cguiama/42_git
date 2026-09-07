# Born2beRoot — checklist da régua de avaliação

Esta checklist foi extraída das sete imagens da régua fornecidas pelo aluno em
7 de setembro de 2026. Ela serve para preparar a defesa; as instruções da régua
descrevem o que o avaliador pedirá e não autorizam alterações automáticas na VM.

O procedimento completo, com comandos, resultados esperados, explicações e
restauração das mudanças, está em `EVALUATION_ROADMAP.md`.

Legenda:

- `[x]`: configurado e comprovado;
- `[~]`: configurado, mas falta praticar exatamente como a régua solicita;
- `[ ]`: pendente para o fechamento final;
- `[E]`: conhecimento que será verificado na simulação oral.

## 1. Preliminares

- [ ] Clonar o repositório na estação de avaliação.
- [ ] Confirmar que somente os arquivos esperados foram enviados.
- [ ] Confirmar `README.md` na raiz.
- [ ] Confirmar `signature.txt` na raiz.
- [ ] Comparar o conteúdo de `signature.txt` com o SHA-1 do VDI usando `diff`.
- [ ] Confirmar que a VM começa desligada.
- [ ] Confirmar que não há snapshots antes do início da avaliação.
- [ ] Iniciar a VM somente depois dessas verificações.

Observação da régua: pode-se criar um snapshot frio dedicado durante a avaliação
e apagá-lo ao final, ou duplicar o VDI e iniciar a cópia. A avaliação não deve
começar com snapshots antigos.

## 2. README

- [x] Primeira linha em itálico com o texto e login exigidos.
- [x] Descrição e objetivo do projeto.
- [x] Instruções de execução.
- [x] Escolha do Debian com vantagens e desvantagens.
- [x] Particionamento, segurança, usuários e serviços.
- [x] Debian versus Rocky Linux.
- [x] AppArmor versus SELinux.
- [x] UFW versus firewalld.
- [x] VirtualBox versus UTM.
- [x] Recursos e descrição do uso de IA.

## 3. Visão geral oral

- [E] Explicar como funciona uma máquina virtual.
- [E] Explicar por que Debian foi escolhido.
- [E] Explicar diferenças principais entre Debian e Rocky.
- [E] Explicar a finalidade das máquinas virtuais.
- [E] Explicar `apt` versus `aptitude`.
- [E] Explicar AppArmor e mostrar seu estado.
- [x] Monitoring aparece no boot e a cada dez minutos.
- [E] Explicar o monitoring linha por linha.

## 4. Configuração simples

- [x] Nenhum ambiente gráfico/X.org/Wayland instalado.
- [x] Login feito com usuário não-root.
- [x] UFW instalado, ativo e habilitado.
- [x] SSH instalado, ativo e habilitado.
- [x] Debian 13.6 stable em funcionamento.
- [E] Saber mostrar cada verificação ao avaliador.

## 5. Usuário — etapa 1

- [x] Usuário `guilamar` existe.
- [x] `guilamar` pertence a `sudo` e `user42`.
- [x] Política de senha configurada.
- [x] Criação de usuário praticada com senha válida.
- [x] Conta nova herdou mínimo 2, máximo 30 e aviso 7.
- [E] Explicar arquivos `/etc/login.defs`,
  `/etc/security/pwquality.conf` e `/etc/pam.d/common-password`.

## 6. Usuário — etapa 2

- [~] Criar exatamente o grupo `avaliando`.
- [~] Adicionar o usuário recém-criado a `avaliando`.
- [~] Mostrar a associação com `id` e `getent`.
- [E] Explicar vantagens e desvantagens da política de senhas.

## 7. Hostname e partições

- [x] Hostname final `guilamar42`.
- [x] Alteração de hostname praticada e restaurada.
- [~] Repetir a alteração com reboot, como a régua exige.
- [~] Confirmar o novo nome após reboot e restaurar o original.
- [x] Estrutura de partições conferida com `lsblk`.
- [x] Estrutura completa do bônus presente.
- [E] Explicar disco, partição, LUKS, PV, VG, LV, filesystem e montagem.
- [E] Explicar a finalidade de cada volume lógico.

## 8. Sudo

- [x] Pacote instalado.
- [~] Adicionar o novo usuário da avaliação ao grupo `sudo`.
- [E] Explicar valor e funcionamento do sudo com exemplos.
- [x] `passwd_tries=3` e mensagem de erro personalizada.
- [x] Entrada e saída registradas.
- [x] TTY obrigatório e `secure_path` restrito.
- [x] `/var/log/sudo/` existe e contém histórico.
- [x] Execução de novo comando atualiza os logs.
- [E] Explicar `sudo.log`, TSID, árvore de I/O e `sudoreplay`.

## 9. UFW

- [x] Instalado, ativo e habilitado.
- [E] Explicar o que é firewall e por que ele é importante.
- [x] Regra TCP 4242 presente.
- [x] Regra TCP 80 presente e justificada pelo bônus.
- [~] Adicionar ao vivo uma regra TCP 8080.
- [~] Verificar que a regra 8080 apareceu.
- [~] Remover a regra TCP 8080 e verificar sua ausência.

Nota: a porta 8080 pedida pela régua é uma porta temporária dentro da VM. Ela não
é a mesma coisa que a porta 8080 do hospedeiro, atualmente redirecionada pelo NAT
para a porta 80 da VM.

## 10. SSH

- [x] Instalado, ativo e habilitado.
- [E] Explicar SSH e sua importância.
- [x] Serviço escuta em 4242, não em 22.
- [x] Login de `root` por SSH foi recusado.
- [x] Login de `guilamar` por SSH funciona.
- [~] Fazer login SSH com o usuário recém-criado durante a avaliação.

## 11. Monitoring e cron

- [x] Script Bash existe em `/usr/local/bin/monitoring.sh`.
- [x] As doze métricas foram verificadas.
- [x] Sem erros de sintaxe.
- [x] Saída enviada a todos os terminais com `wall`.
- [x] Execução no boot e a cada dez minutos via crontab de root.
- [E] Explicar o script mostrando o código.
- [E] Explicar cron e os cinco campos de tempo.
- [~] Alterar a frequência para cada minuto sem editar o script.
- [~] Verificar broadcasts sucessivos com valores dinâmicos.
- [~] Fazer o script parar no boot sem editar o próprio script.
- [~] Reiniciar e comprovar que ele não foi executado automaticamente.
- [~] Verificar após reboot que caminho, permissões e conteúdo não mudaram.
- [~] Restaurar a configuração final de boot e dez minutos.

## 12. Bônus

- [x] Esquema completo de volumes lógicos.
- [x] WordPress funcional.
- [x] Lighttpd, MariaDB e PHP usados.
- [x] Apache2 e Nginx ausentes.
- [x] Fail2ban como serviço adicional.
- [x] Jail SSH configurada para a porta 4242.
- [x] Bloqueio e desbloqueio controlados testados.
- [E] Justificar Fail2ban e explicar sua operação.

Pontuação indicada pela régua:

- particionamento: 2 pontos;
- WordPress: 2 pontos;
- serviço adicional: 1 ponto.

O bônus só é avaliado se toda a parte obrigatória estiver perfeita.

## 13. Ordem dos próximos ensaios

1. Repetir o ciclo de usuário usando grupo `avaliando`, adicionar a `sudo` e
   testar SSH antes da limpeza.
2. Praticar UFW: adicionar e remover TCP 8080.
3. Praticar monitoring: cada minuto, interrupção no boot, reboot e restauração.
4. Praticar hostname com reboot e restauração.
5. Fazer simulação oral completa.
6. Personalizar o WordPress sem alterar a infraestrutura.
7. Confirmar ausência de snapshots, desligar e gerar a assinatura final.

# GhostWindow - A Janela Fantasma 👻

**Uma janela visível apenas para você, invisível em aplicativos de compartilhamento de tela como OBS, Teams, Zoom, Meet e Skype.**

![Demonstração](demo.gif) *(Adicione um GIF de demonstração posteriormente)*

## ✨ Recursos Principais

- 🖥️ **Visível apenas para você** - Aparece normalmente na sua tela  
- 🚫 **Invisível em capturas** - Não aparece no OBS, Teams, Zoom, Meet, Skype, etc.  
- 🎨 **Transparência ajustável** - Teclas `+` e `-` para controle  
- 🖱️ **Arrastável** - Mova com o mouse  
- 📜 **Menu de ajuda** - Pressione `H` para ver os atalhos  
- ✨ **Design minimalista** - Focado em funcionalidade  

## ⌨️ Atalhos do Teclado

| Tecla  | Ação                          |
|--------|-------------------------------|
| `H`    | Mostra/oculta menu de ajuda   |
| `ESC`  | Fecha a janela                |
| `+`    | Aumenta a opacidade (+25%)    |
| `-`    | Diminui a opacidade (-25%)    |

## 🛠️ Como Usar

1. Execute o aplicativo `GhostWindow.exe`
2. A janela aparecerá no canto superior esquerdo da sua tela
3. Controles básicos:
   ```plaintext
   - Arraste com mouse: Segure e arraste para mover
   - Transparência: Use teclas + e -
   - Ajuda: Pressione H
   - Fechar: ESC
   ```
4. Compartilhe sua tela normalmente - a janela permanecerá invisível para outros!
   
## 👥 Compatibilidade com Aplicativos

Compartilhe sua tela normalmente - a janela permanecerá invisível para outros!

**Aplicativos testados e compatíveis:**
- 🎥 OBS Studio (todas as formas de captura)
- 💻 Microsoft Teams (compartilhamento de tela/janela)
- 📹 Zoom Meetings (modos de compartilhamento)
- 🖥 Google Meet
- 📞 Skype
- 🔴 Streamlabs
- 🖌 XSplit Broadcaster

## 📦 Tecnologias Usadas

- Win32 API (C++ puro)
- Técnicas de composição de janelas (WS_EX_LAYERED)
- Windows Display Affinity (WDA_EXCLUDEFROMCAPTURE)
- GDI para renderização

## ⚠️ Limitações Conhecidas


✔️ Windows 10/11 - Compatibilidade total  
⚠️ Windows 8/7 - Funcionalidade limitada (sem suporte a WDA_EXCLUDEFROMCAPTURE)  
🔒 Elevação necessária - Pode requerer execução como Admin em alguns sistemas  
🎚️ Transparência mínima - Limitada a 30% para evitar perda acidental  


## 🌟 Casos de Uso

+ Para streamers: Anotações privadas durante lives
+ Em reuniões: Lembretes pessoais invisíveis
+ Para professores: Roteiro de aula oculto
+ Apresentações: Notas exclusivas para o apresentador
+ Desenvolvedores: Debug overlay privado


## 📥 Compilação

Requisitos:
- GCC/MinGW ou Visual Studio 2019+
- Windows SDK (10 ou 11)


### Método 1: Compilar com g++
```plaintext
g++ src/main.cpp -o GhostWindow.exe -lgdi32 -municode -mwindows
```

### Método 2: Visual Studio
```plaintext
1. Criar novo projeto "Aplicativo da Área de Trabalho do Windows"
2. Adicionar main.cpp ao projeto
3. Configurar para compilar como Release (x86 ou x64)
```

## 📜 Licença

```plaintext
MIT License
Copyright (c) [ano] [seu nome]

Permissão é concedida gratuitamente a qualquer pessoa que obtenha uma cópia
deste software e arquivos de documentação associados [...]
```

## 🛠 Roadmap Futuro

```plaintext
- Adicionar suporte para múltiplas janelas
- Implementar salvamento de posição/tamanho
- Adicionar hotkeys personalizáveis
- Criar versão como serviço/system tray
```

## 💡 Dicas Profissionais

```plaintext
Combine com outras ferramentas para maior privacidade:
- Use com OBS Studio + NDI para transmissões profissionais
- Integre com AutoHotkey para controle automatizado
- Combine com ferramentas de virtualização para camadas extras
```

## ✉️ Contribuições

```plaintext
Contribuições são bem-vindas!
Envie PRs ou reporte issues no GitHub
```


![Windows](https://img.shields.io/badge/Windows-11+-0078D6?logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green)

<!--
<div hidden style="display: none">
### Para completar seu repositório:

```plaintext
1. Crie um arquivo demo.gif mostrando:
   - Ajuste de transparência
   - Movimento da janela
   - Demonstração no OBS/Zoom
2. Adicione um diretório screenshots/ com:
   - Comparativo lado-a-lado (janela visível vs compartilhamento)
3. Inclua um .gitignore para C++/Windows
```
</div>
-->

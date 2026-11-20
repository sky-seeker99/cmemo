<!-- cmemo is a lightweight memo pad and note-taking application for Windows 10 and Windows 11. -->
<!-- It runs in the system tray, supports multiple memos, drag and drop, automatic saving, and remote control. -->
[[japanese]](https://github.com/sky-seeker99/cmemo/blob/main/README_jp.md)

# <img width="290" height="94" alt="cmemo2-2_s" src="https://github.com/user-attachments/assets/6d105a97-e91b-4164-a0f2-ea21d2987ee7" />

![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange.svg)

**A lightweight memo pad that stays in the Windows system tray.**

Jot down quick notes without taking up desktop space, and bring the memo window up whenever you need it. It is a simple note-taking tool that feels like having one more sticky note.

## Features

- 🗂️ **Stays in the system tray**
  - Keep the app in the system tray and open it only when needed.
- 🖱️ **Toggle with a left-click**
  - Left-click the system tray icon to open the memo; left-click again to return it to the tray.
- 💾 **Automatic saving**
  - Memo contents are saved to a file when the program exits.
  - Continue editing your previous notes the next time you launch the program.
- 📝 **Manage multiple memos**
  - Create multiple memos and switch between them for different purposes.
- 📁 **Insert file names**
  - Drag and drop a file into a memo to insert its name.
- ⚙️ **Various settings**
  - Configure the memo list, memo background colors and fonts, and whether windows stay on top.
- 💻 **Remote control**
  - Use the included `cmemo_remote.exe` to show, save, and read memos from an external program.

## System Requirements

- Windows 10 / 11

## Download

- [choi_memo2_099.zip](https://github.com/sky-seeker99/cmemo/releases/download/ver0.99_20260920/choi_memo2_099.zip)

## Installation

This repository currently contains the source code and project files.

To use the executable, check the available releases or distribution packages.

## Basic Usage

1. Launch `cmemo`.
2. The icon appears in the system tray.
3. Left-click the system tray icon to show the memo.
4. Enter your notes.
5. Left-click the system tray icon again to return the memo to the tray.
6. When you exit the program, the memo contents are saved to a file.
7. The next time you launch the program, you can continue using the saved notes.

## Managing Memos

### Add a Memo

Enter a memo name in the single-line field above the memo list, then click the `[new]` button.

You can also left-click the memo list to open its pop-up menu and select `[new]`.

### Delete a Memo

Select the memo you want to delete and click the `[del]` button.

You can also delete a memo using `[del]` in the pop-up menu.

> **Note:** You cannot delete all memos.

### Rename a Memo

Enter the new memo name in the single-line field above the memo list, then click `[ren]`.

You can also rename a memo using `[rename]` in the pop-up menu.

### Reorder Memos

- `[↑]` button: Move the selected memo up.
- `[↓]` button: Move the selected memo down.

## Memo Pop-Up Menu

Left-click in a memo to use the following menu items:

| Menu | Description |
|---|---|
| `[copy]` | Copies the selected text to the clipboard. |
| `[paste]` | Pastes text from the clipboard. |
| `[cut]` | Copies the selected text to the clipboard and deletes it from the memo. |
| `[undo]` | Reverses the most recent edit. |
| `[clear]` | Deletes all memo contents. |
| `[return->crlf]` | Converts line breaks to the correct format if they use CR only. |
| `[setting]` | Opens the settings menu. |
| `[all memo save]` | Saves all memo contents to a file. |
| `[all memo load]` | Loads memo contents saved in a file. |
| `[back color (list)]` | Sets the memo list background color. |
| `[back color (memo)]` | Sets the memo background color. |
| `[font (list)]` | Sets the memo list font. |
| `[font (memo)]` | Sets the memo font. |
| `[version]` | Displays the version. |

### `[setting]` Submenu

| Menu | Description |
|---|---|
| `[list]` | Shows or hides the memo list. |
| `[on top stay]` | Sets whether the window stays on top of other windows. |
| `[file name only]` | Sets whether dropping a file inserts its full path or only its file name. |

## System Tray Menu

Left-click the system tray icon to use the following menu items:

| Menu | Description |
|---|---|
| `[Show]` | Shows the program. |
| `[Exit]` | Exits the program. |

## Buttons Above the Memo

| Button | Description |
|---|---|
| `[home]` | Moves to the top of the memo list. The memo list is hidden. |
| `[list]` | Shows or hides the memo list. |
| `[<<]` | Shows the previous memo in the list. |
| `[>>]` | Shows the next memo in the list. |

## Insert File Names

Drag and drop a file into a memo to insert its name.

Depending on the settings, you can insert either the full path or only the file name.

## Remote Control

You can control `cmemo` from an external program.

Use the included console program, `cmemo_remote.exe`.

### Show a Memo

```text
cmemo_remote.exe memo_name message
```

Use `\n` for a line break.

Example:

```text
cmemo_remote.exe memo1 It's time!
cmemo_remote.exe memo1 "  It's time!\n It's lunchtime."
```

### Minimize All Memos to the System Tray

```text
cmemo_remote.exe -c
```

### Save All Memos to a File

```text
cmemo_remote.exe -s filename
```

Example:

```text
cmemo_remote.exe -s "C:\Documents and Settings\aaa\Desktop\sv.csv"
```

### Load Memos from a File

```text
cmemo_remote.exe -l filename
```

Example:

```text
cmemo_remote.exe -l "C:\Documents and Settings\aaa\Desktop\sv.csv"
```

## Version History

| Version | Changes |
|---|---|
| Ver 0.92 | Initial release. |
| Ver 0.93 | Updated the design. |
| Ver 0.94 | Added memo saving and loading. |
| Ver 0.95 | Separated font and background color settings for the list and memo. Added a button to close the list. |
| Ver 0.96 | Updated the design, hid the program from the taskbar, and fixed bugs in font and background color settings. |
| Ver 0.97 | Added a scrollbar to memos. |
| Ver 0.98 | Fixed a bug that could lose changes to the current memo when navigating with `[<<]` or `[>>]`. |
| Ver 0.99 | Added `[home]`. |
| Ver 0.99a | Changed the installer's compression method. |

## License

This project is distributed under the **Apache License Version 2.0**.

## Issues / Contributing

Please use GitHub Issues to report bugs, request features, or suggest improvements.

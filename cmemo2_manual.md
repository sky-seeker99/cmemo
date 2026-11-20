# Choi Memo II

## 1. About the Program

Choi Memo II is a system tray memo pad for jotting down quick notes.

- Left-click the system tray icon to open the memo pad. Left-click again to minimize it to the tray.
- Memo contents are saved to a file when the program exits. You can continue editing them the next time you start the program.
- You can manage and edit multiple memos.

## 2. Usage

Launch the program to open Choi Memo II.

- The window on the left is the memo list. A `default` memo is included initially.
- The window on the right is the memo editor.

### (1) Add a Memo

Enter the name of the memo you want to add in the single-line field above the memo list.

Then click the `[new]` button to add the memo.

You can also left-click the memo list to open its pop-up menu, then select `[new]` to add a memo.

### (2) Delete a Memo

Select the memo you want to delete in the memo list, then click `[del]`.

You can also left-click the memo list to open its pop-up menu, then select `[del]`.

> **Note:** You cannot delete all memos.

### (3) Rename a Memo

Enter the new name in the single-line field above the memo list.

Then click `[ren]` to rename the memo.

You can also left-click the memo list to open its pop-up menu, then select `[rename]`.

### (4) Move a Memo Up

Select a memo in the list and click `[↑]` to move it up.

### (5) Move a Memo Down

Select a memo in the list and click `[↓]` to move it down.

### (6) Insert a File Name

Drag and drop a file into a memo to insert its name.

### (7) Minimize the Memo to the System Tray

Double-click a memo to minimize it to the system tray.

Left-click the system tray icon to open the memo. Left-click it again to minimize the memo to the tray.

### (8) Pop-Up Menu in a Memo

| Menu | Description |
|---|---|
| `[copy]` | Copies the selected text to the clipboard. |
| `[paste]` | Pastes text from the clipboard. |
| `[cut]` | Copies the selected text to the clipboard and deletes it from the memo. |
| `[undo]` | Reverses the most recent edit. |
| `[clear]` | Deletes all memo contents. |
| `[return->crlf]` | Select this to convert line breaks to the correct format when they use CR only. |
| `[setting]` | Opens the settings menu. |
| `[all memo save]` | Saves all memo contents to a file. |
| `[all memo load]` | Loads memo contents saved in a file. |
| `[back color (list)]` | Selects the memo list window's background color. |
| `[back color (memo)]` | Selects the memo window's background color. |
| `[font (list)]` | Selects the memo list window's font. |
| `[font (memo)]` | Selects the memo window's font. |
| `[version]` | Displays the version. |

#### `[setting]` Submenu

| Menu | Description |
|---|---|
| `[list]` | Shows or hides the memo list. |
| `[on top stay]` | Sets whether the window stays on top of other windows. |
| `[file name only]` | Sets whether dropping a file inserts its full path or only its file name. |

### (9) Pop-Up Menu Opened by Left-Clicking the System Tray Icon

| Menu | Description |
|---|---|
| `[Show]` | Shows the program. |
| `[Exit]` | Exits the program. |

### (10) Buttons Above the Memo

| Button | Description |
|---|---|
| `[home]` | Moves to the top of the memo list. The memo list is hidden. |
| `[list]` | Shows or hides the memo list. |
| `[<<]` | Shows the previous memo in the list. |
| `[>>]` | Shows the next memo in the list. |

## 3. Remote Control

This feature lets you control Choi Memo II from an external program.

Use the included console program, `cmemo_remote.exe`.

### (1) Display a Memo

```text
cmemo_remote.exe memo_name message
```

Use `\n` for a line break.

**Examples:**

```text
cmemo_remote.exe memo1 It's time!!
cmemo_remote.exe memo1 "  It's time!!\n It's lunchtime."
```

### (2) Minimize the Memo to the System Tray

```text
cmemo_remote.exe -c
```

### (3) Save All Memo Contents to a File

```text
cmemo_remote.exe -s filename
```

**Example:**

```text
cmemo_remote.exe -s "C:\Documents and Settings\aaa\Desktop\sv.csv"
```

### (4) Load Memo Contents from a File

```text
cmemo_remote.exe -l filename
```

**Example:**

```text
cmemo_remote.exe -l "C:\Documents and Settings\aaa\Desktop\sv.csv"
```

## 4. Version History

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

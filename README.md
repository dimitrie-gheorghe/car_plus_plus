# car_plus_plus
car_plus_plus is a very low-level text editor. "car" stands for the Romanian "caracter" (English: character).

You can use almost all the common controls shared by most text editors. 
- shift+arrows = custom selection
- ctrl+X = cut
- ctrl+C = copy
- ctrl+V = paste
- ctrl+A = select all

However, don't use ctrl+Z (undo), because it is not yet implemented (and probably it will never be).

To navigate through the menu, use arrows+enter.
Important: when you set the colors, the program ensures that background != text != cursor, so if they are not updated, it is not a bug.

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resources
<!-- renovate: datasource=github-tags depName=SFML/SFML versioning=loose -->
- [SFML](https://github.com/SFML/SFML/tree/3.0.2) (Zlib)
- [portable-file-dialogs](https://github.com/samhocevar/portable-file-dialogs) (WTFPL)

## Linux Dependencies

The file picker dialogs (`Open From Disk`) rely on a native desktop backend to generate graphical dialog overlays. If you are compiling or running the application on Linux (especially within a clean WSL setup or a minimalist window manager like i3, dwm, etc.), ensure you have at least one of the following system utilities installed:

- `zenity` (Standard for GNOME/GTK-based desktops)
- `kdialog` (Standard for KDE-based desktops)
- `matedialog` or `qarma`

On Ubuntu/Debian, you can install the most common backend by running:
```bash
sudo apt update && sudo apt install zenity
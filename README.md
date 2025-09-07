# OOP
#CLI Instruction Set-Command Line Interface Instruction Set
CLI Instruction Set defines all the available commands and flags for interacting with the program through the command line.It serves as a guide for creating, editing, and managing presentation slides directly from the terminal.

## Features
**Slide Managment**
-Add new slides ->create new slides with optional titles.
-Delete slides ->delete slides by index or ID.
-Reorder slides ->change slide positions within the presentation.
-Rename slides titles ->update slide titles anytime.
-List all slides ->Shows all slides with their indexes and titles.
-Select a specific slide ->Selects in which slide to work.
-Resize or move objects on the slide.

**Shape Drawing**
-Draw rectangles, circles, ellipses, lines and polygons with coordinates and size parameters.
-Customize colors (e.g., --color red) for any shape.
-Set border style and thickness for drawn objects.

**Text Handling**
-Insert text boxes anywhere on the slide(add_text --x 100 --y 200).
-Customize text font, size and color
-Edit or remove existing text using object IDs.

**Slide Styling**
-Set background color(set_background --color lightblue).
-Apply background images to slides.
-Add borders and effects to enhance visual appearance.

**Presentation Mode**
- Full-screen slide show
- Navigate using keyboard (Next(n)/Previous(p)/Quit(q))

**File Managment**
-Save presentation as .pptx.
-Open existing files for editing.
-Export slides to PDF for easy sharing.

**CLI Customization**
-Every command supports flags and arguments for flexibility.
-Clear error messages if command are incorrect
-Consistent help command(--help) to display available options.

## Tech Stack
-C++
-OOP
-Command Line Interface
-PPTX generation library

# Serialization Guide

## Overview
Your CLI application now supports saving and loading presentations to/from JSON files!

## New Commands

### Save Command
Save the current presentation to a JSON file:

```
save                    # Saves to default file: presentation.json
save file myfile.json   # Saves to custom file
```

### Load Command
Load a presentation from a JSON file:

```
load file myfile.json   # Load from specified file
```

**Note:** `file` argument is **required** for load command.

## Example Usage

### Complete Workflow:

```
1. addslide              # Create some slides
2. addslide
3. addslide

4. save file test.json   # Save to file
   Output: "Presentation saved successfully to: test.json"

5. removeslide           # Remove a slide

6. load file test.json   # Load from file
   Output: "Presentation loaded successfully from: test.json"
   Output: "Total slides: 3"
```

## JSON Format

The serialization saves:
- **Slides**: ID and shapes
- **Shapes**: Type, position, and properties
  - **Circle**: radius, x, y
  - **Rectangle**: width, height, x, y
  - **Text**: content, fontSize, x, y

Example JSON structure:
```json
{
  "slides": [
    {
      "id": 0,
      "shapes": [
        {
          "type": "Circle",
          "radius": 10.0,
          "x": 0.0,
          "y": 0.0
        },
        {
          "type": "Rectangle",
          "width": 20.0,
          "height": 15.0,
          "x": 5.0,
          "y": 5.0
        }
      ]
    }
  ]
}
```

## Features

✅ **Save to JSON**: Serialize entire presentation
✅ **Load from JSON**: Restore presentation state
✅ **Preserves**: Slide IDs, shapes, positions, properties
✅ **Error Handling**: Clear error messages for file operations

## Notes

- Default save filename: `presentation.json`
- File extension can be anything, but `.json` is recommended
- Loading will **replace** current presentation
- Save/Load operations **cannot be undone**


# Testing Instructions

## Option 1: Automated Tests (Recommended)

### Quick Setup:
1. The test file `TestCommands.cpp` is already in your project
2. **Temporarily change the entry point**:
   - Right-click project → Properties
   - Configuration Properties → Linker → Advanced
   - Set "Entry Point" to: `main` (if not already set)
   - OR temporarily rename `CLI.cpp` to `CLI.cpp.backup`
   - Build the project
   - Run the executable

3. **Or create a separate test project:**
   - Right-click Solution → Add → New Project
   - Create a new Console Application called "Tests"
   - Add `TestCommands.cpp` and necessary header files
   - Set it as startup project
   - Run tests

### What the Automated Tests Check:
- ✅ Command factory registration
- ✅ Add slide command execution
- ✅ Add slide with position argument
- ✅ Remove slide command execution
- ✅ Remove slide with position argument
- ✅ Undo functionality
- ✅ Error handling (invalid commands, empty repository, invalid positions)
- ✅ Command name identification
- ✅ CanUndo functionality
- ✅ Multiple operations sequence

## Option 2: Manual Testing (Interactive)

1. Build and run your main `CLI.exe` program
2. Use the interactive commands:

### Basic Commands:
```
addslide                    # Add a slide
addslide -pos 0            # Add at position 0
removeslide                # Remove last slide
removeslide -at 0          # Remove at position 0
```

### Test Scenarios:

**Scenario 1: Basic Add/Remove**
```
addslide
addslide
removeslide
```
Expected: Should have 1 slide left

**Scenario 2: Position Testing**
```
addslide                   # Add slide 0
addslide -pos 0            # Add at start, now 2 slides
removeslide -at 1          # Remove middle
```
Expected: Should have 1 slide left

**Scenario 3: Error Cases**
```
removeslide                # Should error: empty
addslide
removeslide -at 999        # Should error: out of range
addslide -invalid          # Should error: invalid argument
```

**Scenario 4: Undo Test**
```
addslide
addslide
removeslide -at 0         # Remove first
# (You'll need to store the command and call undo)
```

## Option 3: Quick Test Script

You can also create a test input file and pipe it to your program:
1. Create `test_input.txt`:
```
addslide
addslide
addslide
removeslide -at 1
exit
```

2. Run: `CLI.exe < test_input.txt`

## Viewing Test Results

The automated test will output:
```
========================================
    Command System Test Suite
========================================

[TEST] CommandFactory: Registration Check ... PASSED
[TEST] AddSlideCommand: Basic Execution ... PASSED
...
========================================
Test Results: 12 passed, 0 failed
========================================
```

## Troubleshooting

- **Linker Error (multiple main functions)**: 
  - Exclude `CLI.cpp` temporarily OR
  - Exclude `TestCommands.cpp` from main build
  - Create separate test project

- **Commands not found**: 
  - Make sure `CommandRegistry.cpp` is in the project
  - Check that it's being compiled

- **Build Errors**: 
  - Ensure all files are added to project
  - Check include paths


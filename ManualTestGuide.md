# Manual Testing Guide

## Quick Start
1. Build the project
2. Run `CLI.exe`
3. Test commands interactively

## Test Commands to Try

### 1. Add Slide Commands
```
addslide                    # Add slide at end (default)
addslide -pos 0            # Add slide at position 0
addslide -pos 1            # Add slide at position 1
```

### 2. Remove Slide Commands
```
removeslide                 # Remove last slide (default)
removeslide -at 0          # Remove slide at position 0
removeslide -at 1          # Remove slide at position 1
```

### 3. Error Cases to Test
```
removeslide                 # Should fail if no slides exist
addslide -pos 999          # Should fail if position too large
removeslide -at 999        # Should fail if index out of range
addslide -invalid arg      # Should fail with invalid argument
```

### 4. Sequence Test
```
addslide                    # Add slide 1
addslide                    # Add slide 2
addslide                    # Add slide 3
removeslide -at 1          # Remove middle slide
# Should have 2 slides remaining
```

## Expected Output

### Successful Command
```
Slide added at position 0
Total slides: 1

Enter command:
```

### Error Output
```
Error: Cannot remove slide: repository is empty

Enter command:
```

## Interactive Test Scenario

1. **Start**: Empty presentation
2. **Add 3 slides**: `addslide` three times
3. **Check count**: Should see "Total slides: 3"
4. **Remove middle**: `removeslide -at 1`
5. **Verify**: Should have 2 slides
6. **Remove first**: `removeslide -at 0`
7. **Verify**: Should have 1 slide
8. **Remove last**: `removeslide`
9. **Verify**: Should be empty
10. **Try remove empty**: `removeslide` (should error)


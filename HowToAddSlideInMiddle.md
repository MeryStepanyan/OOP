# How to Add Slide in the Middle

## The Issue
You can add slides in the middle! Here's how:

## Correct Command Format

### To add in the middle:
```
addslide pos 1    # Add at position 1 (middle if you have 2+ slides)
addslide pos 0    # Add at position 0 (beginning)
```

### Examples:

**Example 1: Add in the middle**
```
1. addslide           # Adds slide at position 0 (count: 1)
2. addslide           # Adds slide at position 1 (count: 2)
3. addslide pos 1     # INSERTS at position 1, pushing others (count: 3)
```
Result: You'll have 3 slides, with the new one in the middle!

**Example 2: Add at beginning**
```
1. addslide           # Adds slide at position 0
2. addslide           # Adds slide at position 1  
3. addslide pos 0     # INSERTS at beginning, pushing others
```
Result: New slide at position 0, others shifted right

**Example 3: Insert multiple times**
```
1. addslide           # Position 0
2. addslide           # Position 1  
3. addslide           # Position 2
4. addslide pos 1     # Insert at middle (position 1)
5. addslide pos 2     # Insert at new position 2
```
Result: Multiple slides inserted in the middle

## Understanding Positions

- **Position 0** = Beginning (first slide)
- **Position 1** = After first slide
- **Position 2** = After second slide
- **Position N** = After Nth slide (or end if N equals count)

When you INSERT at a position, all slides at that position and after are shifted right!

## Valid Position Range

If you have N slides:
- Valid positions: 0 to N (inclusive)
- Position 0 = Insert at beginning
- Position 1 to N-1 = Insert in middle
- Position N = Insert at end (same as default)

## Troubleshooting

### Error: "Position exceeds slide count"
**Cause:** You're trying to insert at position that's too high
**Solution:** Valid positions are 0 to (current slide count)
**Example:** If you have 2 slides, valid positions are 0, 1, or 2

### Command not recognized?
**Make sure format is:**
- `addslide pos 1` (with space, not dash)
- OR `addslide -pos 1` (dash is optional but works)

### Want to see current slides?
The command prints total slides after each add:
```
Slide added at position 1
Total slides: 3
```

## Quick Test Sequence

Try this to see it working:

```
addslide           # Adds 1st slide
addslide           # Adds 2nd slide  
addslide pos 1     # INSERTS new slide between 1st and 2nd
                   # Now you have 3 slides with new one in middle!
```


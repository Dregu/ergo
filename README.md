# ergo

Fork of a minimal status bar for Wayland compositors.

## Usage

Use stdin to pass pango markup for display, prefixed by a column number and any separator. Also `ergo -h`.

### Examples

```
while true; do
  echo 1,stuff on left
  echo 2,"<span color='red'>important stuff</span>"
  echo 3,$(date +%T)
  sleep 1
done | ergo -c 3
```

- [this thing](https://github.com/Dregu/paetkis/)

# 🎵 Music Playback Queue — Doubly Linked List in C++

A real-world implementation of a music player queue built from scratch using a doubly linked list. No arrays, no STL containers — just raw pointer manipulation.

This started as a data structures exercise but turned into something more interesting once I tried to handle actual player behavior: mid-queue insertions, shuffling without losing the current track, and safe deletions around an active node.

---

## 💡 The Problem

Most linked list tutorials cover insert-at-head and delete-at-tail. Music players don't work like that.

A real queue needs to handle:
- A user hitting **"Play Next"** — song goes right after the current one, not at the end
- **Skipping** forward while maintaining history
- **Removing** any song — including ones right next to whatever's playing
- **Shuffling** the upcoming songs without interrupting the current track
- **Clearing** everything ahead when the user wants a fresh queue

The tricky part: a `current` pointer that must **never dangle** across any of these operations.

---

## 📁 Project Structure

```
music-queue/
├── music_queue.cpp     # Full implementation + main() demo
└── README.md
```

---

## ⚙️ How It Works

### Node Structure
```cpp
struct Node {
    string song;
    Node* next;
    Node* prev;
    Node(string name) {
        song = name;
        next = prev = nullptr;
    }
};
```

### Core Operations

| Function | What it does | Edge case handled |
|---|---|---|
| `playNext(song)` | Insert after `current` | current at tail |
| `skip()` | Move current forward | current is last node |
| `remove(song)` | Delete any node by name | node is current / neighbor of current |
| `shuffle()` | Randomize all except current | current stays in place |
| `clearAhead()` | Delete everything after current | nothing ahead |
| `display()` | Print queue, mark current | empty queue |

---

## 🚀 Run It

```bash
g++ -o music_queue music_queue.cpp
./music_queue
```

Expected output:
```
--- Playback Queue ---
  >> Pasoori (now playing)
     Kana Yaari
     Coke Studio Season 14
     Tu Jhoom
     Afreen Afreen
----------------------
```

---

## 🧠 What I Learned

**The `current` pointer is the real challenge.** Every single operation has to ask: *does this affect current?*

- `remove()` — if you delete current's neighbor, you must relink before the node is freed
- `shuffle()` — you can't just reorder nodes blindly; current has to anchor in place
- `playNext()` — inserting in the middle means handling 4 pointer connections, not 2

The shuffle implementation pulls all non-current nodes into a `vector`, shuffles them with `mt19937`, then relinks the entire list with current placed in the middle. Not the most elegant solution but it gets the job done without a secondary data structure for the list itself.

---

## 🔧 Possible Extensions

- [ ] `previous()` — go back to the last played song (needs a history stack)
- [ ] `addToEnd(song)` — append without affecting current position  
- [ ] Priority queue — urgent songs jump to right after current
- [ ] Circular mode — tail connects back to head

---

## 💬 Discussion

If you're solving something similar, I'm curious how you'd handle the `current` anchor differently. A sentinel node? A wrapper struct that owns the pointer? Open an issue or drop a comment.

---

*Built while learning data structures. Feedback welcome.*

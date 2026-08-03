# 👑 GNL-GOD (Version 2026)

<p align="center">
  <img src="https://img.shields.io/badge/Project-get__next__line-blueviolet?style=for-the-badge&logo=42" alt="42 Project">
  <img src="https://img.shields.io/badge/Tester-God%20Edition-gold?style=for-the-badge" alt="God Edition">
</p>

A professional, ultra-fast, and visually stunning tester for the 42 **get_next_line** project. Re-engineered to align perfectly with the **2024-2026 Subject** requirements, focusing on performance, memory leaks, and multiple file descriptor handling.

---
> [!WARNING]
> ### ⚖️ Academic Integrity & Official Disclaimer
> PLEB42 is an entirely **independent community initiative**. It is **not** officially endorsed by, affiliated with, or sponsored by the Staff of 42 or 42 São Paulo.
> * **Zero Solution Hosting:** None of our repositories will ever host, share, or leak source code solutions for curriculum assignments. Our engineering scope is strictly limited to behavior validation, crash identification, and memory leak analysis.
> * **No Academic Verdicts:** Passing a PLEB42 test suite provides **no guarantee** of passing the official Moulinette evaluation. Cadets bear ultimate and sole responsibility for the integrity, compliance, and originality of their submissions.

> [!CAUTION]
> ### 🚨 Academic Integrity & Peer-Review Warning 
> **This tester is designed to be intentionally rigorous by nature (overkill).** It tests extreme edge cases that often go beyond the official curriculum requirements. 
> * **Do not fail your peers unfairly:** A failure in this test suite **does not** mean a student should fail their peer-review.
> * **Evaluation Criteria:** If your peer's code functions correctly and respects the official project *subject*, it deserves a pass. Use this tool for learning, not as an absolute grading metric.
---

## ✨ Features

- 🚀 **Real-time God Progress:** Watch your tests pass in real-time with an in-place updating progress bar.
- 🎯 **Multi-Scenario Testing:** Automatic testing across multiple **BUFFER_SIZE** configurations (1, 42, 9999, 10M).
- 💎 **Academic Integrity:** Designed to be a rigorous validator. It reports discrepancies and memory leaks clearly.
- 🧪 **Extreme Robustness:** Tests includes empty files, files with/without newlines, long lines, and stdin.
- 📂 **Refined Deepthought Logs:** Professional test reports showing exact results and memory allocation details.
- 🛡️ **Leak Detection:** Integrated memory leak checking using the highly reliable **gnlTester** core.
- ⚙️ **Auto-Detection Config**: Dynamically detects if GNL is already in the parent directory and initializes path configurations on first run without manual setup.

---

## 🚀 Installation

1. **Clone** this repository inside your `get_next_line` folder:
   ```bash
   git clone https://github.com/PLEB42/gnl-god.git god_tester
   ```

2. **Run** the magic:
   ```bash
   cd god_tester
   ./grademe.sh
   ```
   *(On the first run, GNL-God will automatically detect your project files in the parent directory and configure `my_config.sh` dynamically).*

---

## 🎮 Usage

### Basic Execution (Runs Mandatory + Bonus Parts)
```bash
./grademe.sh
```

### Doom / Hardcore Mode (Extreme Stress Tests)
```bash
./grademe.sh -doom
```

### Targeted Testing
| Flag | Description |
| :--- | :--- |
| `./grademe.sh -mandatory` | Runs ONLY the mandatory part tests (basic). |
| `./grademe.sh -bonus` | Runs ONLY the bonus part (multiple FDs, basic). |
| `./grademe.sh -stress` | Runs ONLY the doom/hardcore part (extreme tests). |

### Utility Options
- `./grademe.sh -n`: Skip Norminette.
- `./grademe.sh -f`: Skip Forbidden functions check.
- `./grademe.sh -a` or `./grademe.sh --about`: Display the Pleb42 about screen.

---

## 📊 The Deepthought Report

The generated `deepthought` file is your best friend for debugging. It follows a clean format:
- **Scenario:** Shows the BUFFER_SIZE and the file being tested.
- **Visual Emojis**: Displays test outcomes cleanly with emojis (`1.OK ✅`, `1.KO ❌`, `1.MOK 👑`, `1.MKO 🚨`), making it highly readable.
- **Leaks:** Detailed leak report if any allocation was not freed.

---

## 📜 Credits

Part of the **God Series** for 42. Inspired by `libft-god` and `ft_printf-god`, using the robust testing core of `gnlTester`.

<p align="center">
  Made with ❤️ for the 42 Community.
</p>

# Year 2038 Problem Demo

A demonstration of the [Year 2038 problem](https://en.wikipedia.org/wiki/Year_2038_problem) showing how 32-bit time storage overflows.

## 🕐 What is the Year 2038 Problem?

The [Year 2038 problem (Y2038)](https://en.wikipedia.org/wiki/Year_2038_problem) is similar to the famous [Y2K Problem](https://en.wikipedia.org/wiki/Year_2000_problem), but affects the year 2038 instead of 2000.

**The Issue:**

- Computers store time as seconds since **January 1, 1970** (Unix epoch)
- Many systems use a **32-bit signed integer** for this storage
- **Maximum value**: `2,147,483,647` seconds = **January 19, 2038 at 03:14:07 UTC**
- **What happens next**: Integer overflow causes time to "jump back" to **December 13, 1901**

**Why This Matters:**

- Legacy systems/devices still use 32-bit time storage
- Applications may crash, show wrong dates, or behave unpredictably
- Critical infrastructure could be affected

## 🚀 Quick Start

### Prerequisites

- C compiler (GCC, Clang, or similar)
- POSIX-compliant system (Linux, macOS, Unix)

### Build and Run

```bash
make
```

## 📊 Sample Output

```plaintext
Input: 1970-01-01T00:00:00Z
+------------------+----------------------+-------------------------------------------------------------------------+
| Time stored in   | Actual Result        | Binary Representation                                                   |
+------------------+----------------------+-------------------------------------------------------------------------+
| time_t (64-bit)  | 1970-01-01T00:00:00Z | 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 |
| time_t (32-bit)  | 1970-01-01T00:00:00Z | 00000000 00000000 00000000 00000000                                     |
+------------------+----------------------+-------------------------------------------------------------------------+

Input: 2025-08-24T01:23:45Z
+------------------+----------------------+-------------------------------------------------------------------------+
| Time stored in   | Actual Result        | Binary Representation                                                   |
+------------------+----------------------+-------------------------------------------------------------------------+
| time_t (64-bit)  | 2025-08-24T01:23:45Z | 10100001 01101001 10101010 01101000 00000000 00000000 00000000 00000000 |
| time_t (32-bit)  | 2025-08-24T01:23:45Z | 10100001 01101001 10101010 01101000                                     |
+------------------+----------------------+-------------------------------------------------------------------------+

Input: 2038-01-19T03:14:07Z
+------------------+----------------------+-------------------------------------------------------------------------+
| Time stored in   | Actual Result        | Binary Representation                                                   |
+------------------+----------------------+-------------------------------------------------------------------------+
| time_t (64-bit)  | 2038-01-19T03:14:07Z | 11111111 11111111 11111111 01111111 00000000 00000000 00000000 00000000 |
| time_t (32-bit)  | 2038-01-19T03:14:07Z | 11111111 11111111 11111111 01111111                                     |
+------------------+----------------------+-------------------------------------------------------------------------+

Input: 2038-01-19T03:14:08Z
+------------------+----------------------+-------------------------------------------------------------------------+
| Time stored in   | Actual Result        | Binary Representation                                                   |
+------------------+----------------------+-------------------------------------------------------------------------+
| time_t (64-bit)  | 2038-01-19T03:14:08Z | 00000000 00000000 00000000 10000000 00000000 00000000 00000000 00000000 |
| time_t (32-bit)* | 1901-12-13T20:45:52Z | 00000000 00000000 00000000 10000000                                     |
+------------------+----------------------+-------------------------------------------------------------------------+

Note: Row marked with * shows the Y2038 overflow problem.
```

## 🤝 Contributing

Feel free to submit issues or pull requests to improve this educational demonstration.

## 📄 License

This project is provided for educational purposes. Feel free to use and modify as needed.

# README

This is a example of a custom message handler.
Can be used with Qt5 and Qt6.

## How it works

- For simplicity reason everything is in main.cpp
- Consider to make your own class of it

- You can write into a log file or print on std out using the flag: `--consolelog`
- You can define custom log level formats
- And easily configure them  with `setLogMessageFormat(LogMessageFormat::<Formats>)`;
- Formats are FullColor, Full, Standard, Minimal, None

Example output:
LogMessageFormat::Full / LogMessageFormat::FullColor
```
2025-01-15 11:00:08  [debug] Nothing to see here ... (<path_to_project>/MessageHandler/main.cpp:130)
2025-01-15 11:00:08  [info] For your information ... (<path_to_project>/MessageHandler/main.cpp:131)
2025-01-15 11:00:08  [warning] This is the final warning! (<path_to_project>/MessageHandler/main.cpp:132)
2025-01-15 11:00:08  [critical] Houston we have a big problem! (<path_to_project>/MessageHandler/main.cpp:133)
```

## Colored logs

- As Qt does not support colored outputs natively there is a hack using ASCII excape sequences.
- Obviously this will not work for log messages written files.
- But you can print colored console logs when using: `setLogMessageFormat(LogMessageFormat::FullColor);`
- Therefor the `--consolelog` flag has to be passed when starting your application: `<myapp> --consolelog`

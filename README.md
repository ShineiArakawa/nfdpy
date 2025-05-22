# nfdpy: A Python Binding for nativefiledialog-extended

![teaser.png](/assets/teaser.png)

This repository provides a Python binding for the [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended) library, which is a awesome library for using OS-native file dialogs in a cross-platform way, developped by [Bernard Teo](https://github.com/btzy).

## Installation

You can install the package using pip:

```bash
pip install git+https://github.com/ShineiArakawa/nfdpy.git
```

## Example Usage

```python
import nfdpy

selected = nfdpy.open_file_dialog(
    filters={
        "C++ files": "cpp,hpp",
        "Python files": "py",
    },
)

print("Selected file:", selected)
```

## License

See [LICENSE](/LICENSE).

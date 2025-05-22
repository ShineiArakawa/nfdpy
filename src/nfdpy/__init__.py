"""A Python bindings for 'nativefiledialog-extended' library

Contact
-------

- Author: Shinei Arakawa
- Email: sarakawalab@gmail.com

"""

# ----------------------------------------------------------------------------
# Check Python version

import sys

if sys.version_info < (3, 10):
    raise ImportError("Python 3.10 or higher is required.")

# ----------------------------------------------------------------------------
# Check the version of this package

import importlib.metadata

try:
    __version__ = importlib.metadata.version("research_utilities")
except importlib.metadata.PackageNotFoundError:
    # package is not installed
    __version__ = "0.0.0"

# ----------------------------------------------------------------------------
# Import modules

from .binding import open_file_dialog, open_multi_file_dialog

__all__ = [
    "open_file_dialog",
    "open_multi_file_dialog",
]

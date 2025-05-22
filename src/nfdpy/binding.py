import os

from nfdpy import _nfdpy


def open_file_dialog(filters: dict[str, str] = {}, default_path: str | None = None) -> str | None:
    """Open a file dialog and return the selected file path.

    Parameters
    ----------
    filters : dict[str, str], optional
        File filters for the dialog, by default {}
        You can specify the file type label as the key and the file extensions as the value.

        For example,
        ```
        filters = {
            "C++ files": "cpp,hpp",
            "Python files": "py",
            "Text files": "txt",
        }
        ```
    default_path : str | None, optional
        The default directory to open the dialog in or default file name to suggest, by default None
        If None, the current working directory will be used.

    Returns
    -------
    str
        The selected file path. If the user cancels the dialog, `None` is returned.
    """

    vec_filters: list[list[str, str]] = [[str(k), str(v)] for k, v in filters.items()]

    if default_path is None:
        default_path = os.getcwd()

    is_OK, selected = _nfdpy.open_file_dialog(vec_filters, default_path)

    if not is_OK:
        return None

    return str(selected)


def open_multi_file_dialog(filters: dict[str, str] = {}, default_path: str | None = None) -> list[str] | None:
    """Open a multi-file dialog and return the selected file paths.

    Parameters
    ----------
    filters : dict[str, str], optional
        File filters for the dialog, by default {}
        You can specify the file type label as the key and the file extensions as the value.

        For example,
        ```
        filters = {
            "C++ files": "cpp,hpp",
            "Python files": "py",
            "Text files": "txt",
        }
        ```
    default_path : str | None, optional
        The default directory to open the dialog in or default file name to suggest, by default None
        If None, the current working directory will be used.

    Returns
    -------
    list[str] | None
        The selected file paths. If the user cancels the dialog, `None` is returned.
        If multiple files are selected, a list of file paths is returned.
    """

    vec_filters: list[list[str, str]] = [[str(k), str(v)] for k, v in filters.items()]

    if default_path is None:
        default_path = os.getcwd()

    is_OK, selected = _nfdpy.open_multi_file_dialog(vec_filters, default_path)

    if not is_OK:
        return None

    return [str(s) for s in selected]

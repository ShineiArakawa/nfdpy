import nfdpy


def main():
    selected = nfdpy.open_multi_file_dialog(
        filters={
            "C++ files": "cpp,hpp",
            "Python files": "py",
            "Text files": "txt",
            "Video files": "mp4,mkv",
            "Image files": "jpg,jpeg,png,gif",
        },
    )

    print("Selected file:", selected)


if __name__ == "__main__":
    main()

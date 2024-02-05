# NITCbase

NITCbase is an object-oriented implementation of a relational database management system (RDBMS) created as part of a college project.

![NITCbase Logo](https://github.com/NITCbase/nitcbase.github.io/raw/main/static/img/FrontBannerLogo.png)

## Overview

NITCbase offers a user-friendly interface to interact with the RDBMS. You can explore the list of available commands and features in the [User Interface Commands Documentation](https://nitcbase.github.io/docs/User%20Interface%20Commands/).

## Installation

To get started with NITCbase, follow these installation steps:

1. Clone the repository:

    ```sh
    git clone https://github.com/vikram18rock/NITCbase.git
    ```

2. Navigate to the project directory:

    ```sh
    cd NITCbase/mynitcbase
    ```

3. Build the project using the `make` command:

    ```sh
    make
    ```

    You can also use the following make options:
    
    - `make clean`: Clean up the project.
    - `make mode=debug`: Create a debuggable version.

## Usage

Once the installation is complete, you can use NITCbase to interact with the RDBMS. NITCbase provides a user-friendly command-line interface to work with relational databases. You can refer to the [User Interface Commands Documentation](https://nitcbase.github.io/docs/User%20Interface%20Commands/) to learn about the available commands and their usage.

For detailed usage instructions and information about the XFS interface, please refer to the [Xfs Interface README](XFS_Interface/README.md). You can find instructions on how to build, clean, and debug the XFS interface there.

Be aware that the XFS interface operates directly on the disk without creating a run copy.

## Project Structure

- [`Disk/`](Disk): Contains binary files disk and disk_run_copy.
- [`Files/`](Files): Contains the input and output files. You can also write scripts in the Batch_exec_files folder to execute multiple commands at once.
- [`mynitcbase/`](mynitcbase): The main project directory.
- [`XFS_Interface/`](XFS_Interface): Contains the source code for the XFS interface.

## Contributing

We welcome contributions from the open-source community. If you'd like to contribute to NITCbase, please follow our [Contribution Guidelines](CONTRIBUTING.md).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.md) file for details.

## Contact

For questions, feedback, or support, please feel free to contact us:

- Email: [v16ash@gmail.com](mailto:v16ash@gmail.com)
- Project Website: [https://nitcbase.github.io](https://nitcbase.github.io)
- GitHub Repository: [https://github.com/vikram18rock/NITCbase](https://github.com/vikram18rock/NITCbase)

Thank you for your interest in NITCbase!

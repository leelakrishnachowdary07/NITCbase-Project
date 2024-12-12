# NITCbase

NITCbase is an object-oriented implementation of a relational database management system (RDBMS) created as part of a college project.

![NITCbase Logo](https://github.com/NITCbase/nitcbase.github.io/raw/main/static/img/FrontBannerLogo.png)


---

# **NITCbase: A Relational Database Management System**

## **Project Overview**  
NITCbase is an educational Relational Database Management System (RDBMS) designed to demonstrate the inner workings of database systems. It is organized into distinct layers, each responsible for specific functionalities, mirroring the modular design of modern RDBMS. The project was implemented using C++ and emphasizes both theoretical understanding and practical implementation.

---

## **Key Features and Layer Functionality**

1. **Frontend Layer**  
   - **Purpose**: Acts as the user interface, translating SQL-like queries into system-recognized commands.  
   - **Key Functionality**:  
     - Converts user inputs into internal function calls.  
     - Ensures seamless interaction between the user and the database system.  

2. **Schema Layer**  
   - **Purpose**: Handles database structure management through Data Definition Language (DDL) commands.  
   - **Key Functionality**:  
     - Create, delete, rename, and modify tables and attributes.  
     - Manages metadata using lower layers for disk access and indexing.  

3. **Algebra Layer**  
   - **Purpose**: Processes Data Manipulation Language (DML) commands for querying and modifying data.  
   - **Key Functionality**:  
     - Implements SELECT, PROJECT, and JOIN operations.  
     - Prepares complex queries for execution by lower layers.  

4. **Cache Layer**  
   - **Purpose**: Maintains in-memory structures for frequently accessed data to enhance performance.  
   - **Key Functionality**:  
     - Manages relation and attribute caches.  
     - Facilitates quick access to metadata and runtime data.  

5. **Block Access Layer**  
   - **Purpose**: Core layer for retrieving and manipulating data stored on disk.  
   - **Key Functionality**:  
     - Executes fundamental DML operations (select and project).  
     - Provides data access for higher layers, ensuring consistency and efficiency.  

6. **B+ Tree Layer**  
   - **Purpose**: Optimizes data retrieval using indexing techniques.  
   - **Key Functionality**:  
     - Implements B+ Tree search and insertion operations.  
     - Enhances query performance by reducing data retrieval time.  

7. **Buffer Layer**  
   - **Purpose**: Manages disk buffering and provides an interface for accessing disk blocks.  
   - **Key Functionality**:  
     - Implements a Least Recently Used (LRU) buffer replacement strategy.  
     - Handles efficient caching and disk I/O operations.  

8. **Physical Layer**  
   - **Purpose**: Provides low-level disk access routines.  
   - **Key Functionality**:  
     - Reads data from and writes data to disk blocks.  
     - Acts as the foundation for all higher-layer operations.  

---

## **Technologies Used**  
- **Programming Language**: C++  
- **Core Concepts**: Query processing, indexing, caching, buffer management, and disk I/O operations.  

---

## **Challenges and Learnings**  
- Implementing efficient B+ Trees and managing runtime caches.  
- Ensuring smooth integration and communication across multiple layers.  
- Optimizing disk access through caching and buffering techniques.  

---

## **Future Improvements**  
- Incorporating advanced query optimization techniques.  
- Adding transaction management with concurrency control.  
- Enhancing scalability through distributed storage or partitioning.

---


NITCbase offers a user-friendly interface to interact with the RDBMS. You can explore the list of available commands and features in the [User Interface Commands Documentation](https://nitcbase.github.io/docs/User%20Interface%20Commands/).

## Installation

To get started with NITCbase, follow these installation steps:

1. Clone the repository:

    ```sh
    git clone https://github.com/leelakrishnachowdary07/NITCbase-Project.git
    ```

2. Navigate to the project directory:

    ```sh
    cd NITCbase-Project/DBMS/NITCbase/mynitcbase
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


## Contact

For questions, feedback, or support, please feel free to contact us:

- Email: [leelakrishnachowdary21171@gmail.com](mailto:leelakrishnachowdary21171@gmail.com)
- Project Website: [https://nitcbase.github.io](https://nitcbase.github.io)
- GitHub Repository: [https://github.com/leelakrishnachowdary07/NITCbase-Project](https://github.com/leelakrishnachowdary07/NITCbase-Project)

Thank you for your interest in NITCbase-Project!

# DNN-Based Image Classification on ARM (Zybo Board)

## Overview
This repository presents a comprehensive exploration into deploying a micro-service powered by a Deep Neural Network (DNN) for the classification of handwritten digits on the Zybo Z7 board. Designed for environments with limited computational and memory resources, this project demonstrates the potential of optimizing a neural network to efficiently classify images using binary files for weights, biases, and input images.

## Project Structure
* **The System**:  Introduction to the application, including the Zybo Z7 board setup, system characteristics, and the DNN model specifically tailored for handwritten digit classification.
* **Baseline Performance**: Detailed analysis of the baseline performance, including execution times, memory footprint, and accuracy without optimizations.
* **Optimizations**:
    * **NEON Autovectorization**: Enhancements made by leveraging ARM NEON technology for auto-vectorization, improving execution time and efficiency.
    * **NEON Intrinsics**: Manual optimization using NEON intrinsics for finer control over performance enhancements.
    * **Reduced Data Representation**: Optimization strategies focusing on minimizing memory usage by adjusting data representation, with an in-depth examination of impacts on accuracy and performance.
* **Performance of Optimizations**: Comparative analysis of different optimization strategies, including execution times, GOPS/s, and their effects on DNN accuracy.
* **Conclusions**: Final thoughts on the deployment of the DNN on the Zybo board, including a discussion on the feasibility of optimizations and their implications for similar embedded systems.

## Highlights
* **Practical Deployment**: Insights into the practical aspects of deploying a neural network on an embedded system with limited resources.
* **Optimization Techniques**: Exploration of various optimization techniques, including compiler settings, ARM NEON technology, and memory footprint management.
* **Comparative Analysis**: Evaluation of different optimization methods to identify the most effective strategies for performance enhancement and memory efficiency.

## Getting Started
Instructions for setting up the project, including requirements for the Zybo Z7 board, software tools (Xilinx SDK, HexEditor, RealTerm), and steps to compile and run the application.

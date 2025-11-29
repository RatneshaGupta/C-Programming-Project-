# Bank Loan Calculator

This project provides a command-line based Bank Loan Calculator implemented in C. It supports EMI computation, loan summaries, basic amortization previews, and full amortization schedule export in CSV format.

# Overview

The program allows users to calculate the Equated Monthly Installment (EMI) for a loan based on principal amount, annual interest rate, and loan tenure. It also provides the option to preview the first 12 months of the amortization schedule and export the full repayment schedule to a CSV file.

# The EMI calculation follows the standard formula:

𝐸
𝑀
𝐼
=
𝑃
⋅
𝑟
(
1
+
𝑟
)
𝑛
(
1
+
𝑟
)
𝑛
−
1
EMI=
(1+r)
n
−1
P⋅r(1+r)
n
	​


Where:

𝑃
P — Principal amount

𝑟
r — Monthly interest rate (annual rate / 12 / 100)

𝑛
n — Total number of months

In cases where the interest rate is 0%, EMI defaults to:

𝐸
𝑀
𝐼
=
𝑃
𝑛
EMI=
n
P
	​

# Features

Calculation of EMI based on user input.

Summary of loan details, including total payment and total interest.

Amortization preview for the first 12 months.

Full amortization schedule export to a CSV file.

Quick EMI computation mode using years instead of months.

Input validation and error handling for incorrect or invalid user entries.

# Menu Functions
1. Detailed Loan Input

Allows the user to enter:

Principal amount

Annual interest rate (%)

Tenure (months or years)

# Provides:

EMI calculation

Total interest and total amount payable

Optional amortization preview

Optional CSV schedule export

2. Quick EMI Calculation

Allows fast EMI calculation based on principal, interest rate, and tenure in years.

0. Exit

Terminates the program.

Program Structure

# Key functions include:

compute_emi() — EMI calculation

show_summary() — Displays summary of repayment details

show_preview() — Displays amortization preview

save_schedule() — Writes full amortization schedule to CSV

Input helper functions for integers, doubles, and yes/no prompts

# Error Handling

The program includes:

Validation for negative or zero values

Input sanitation for invalid numeric entries

Controlled handling of rounding issues during the final payment month

# CSV Output

The exported CSV schedule contains the following columns:

Month, EMI, Interest, Principal, Balance


Each row represents one month of the loan’s amortization schedule.

//Bank Loan Calculator
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 256

/*  prototypes */
void print_header(void);
void print_menu(void);
int read_int(const char *prompt, int *out);
int read_double(const char *prompt, double *out);
double compute_emi(double principal, double annual_rate_percent, int months);
void show_summary(double principal, double annual_rate_percent, int months);
void show_preview(double principal, double annual_rate_percent, int months);
int yes_no(const char *prompt);
void save_schedule(double principal, double annual_rate_percent, int months, const char *fname);

/* Main entry */
int main(void) {
    print_header();
    while (1) {
        print_menu();
        int choice = 0;
        if (!read_int("Enter choice: ", &choice)) {
            printf("Invalid input. Try again.\n");
            continue;
        }
        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        }
        if (choice == 1) {
            double principal = 0.0;
            double rate = 0.0;
            int months_or_zero = 0;
            if (!read_double("Loan principal (e.g. 500000): ", &principal)) continue;
            if (principal <= 0.0) { printf("Principal must be positive.\n"); continue; }
            if (!read_double("Annual interest rate %% (e.g. 6.5): ", &rate)) continue;
            if (rate < 0.0) { printf("Rate cannot be negative.\n"); continue; }
            if (!read_int("Tenure in months (or 0 to enter years): ", &months_or_zero)) continue;
            int months = 0;
            if (months_or_zero == 0) {
                int years = 0;
                if (!read_int("Tenure in years: ", &years)) continue;
                if (years <= 0) { printf("Years must be positive.\n"); continue; }
                months = years * 12;
            } else {
                months = months_or_zero;
                if (months <= 0) { printf("Months must be positive.\n"); continue; }
            }
            show_summary(principal, rate, months);
            if (yes_no("Show amortization preview (first 12 months)? (y/n): ")) {
                show_preview(principal, rate, months);
            }
            if (yes_no("Save full schedule to CSV? (y/n): ")) {
                char fname[MAX_LINE];
                printf("Filename (Enter for schedule.csv): ");
                if (!fgets(fname, sizeof(fname), stdin)) {
                    printf("Read error. Skipping save.\n");
                } else {
                    fname[strcspn(fname, "\n")] = '\0';
                    if (strlen(fname) == 0) strcpy(fname, "schedule.csv");
                    save_schedule(principal, rate, months, fname);
                }
            }
        } else if (choice == 2) {
            double p, r;
            int y;
            if (!read_double("Loan principal: ", &p)) continue;
            if (!read_double("Annual rate %: ", &r)) continue;
            if (!read_int("Tenure in years: ", &y)) continue;
            if (y <= 0) { printf("Years must be positive.\n"); continue; }
            int m = y * 12;
            double emi = compute_emi(p, r, m);
            printf("\nQuick Result:\n");
            printf("Monthly EMI = %.2f for principal %.2f at %.2f%% for %d years\n", emi, p, r, y);
        } else {
            printf("Please choose 0, 1, or 2.\n");
        }
    }
    return 0;
}

void print_header(void) {
    printf("========================================\n");
    printf("   Bank Loan Calculator \n");
    printf("========================================\n\n");
}

/* Print menu options */
void print_menu(void) {
    printf("\nMain Menu:\n");
    printf("1) Detailed flow: input principal, rate, tenure + options\n");
    printf("2) Quick EMI (enter years)\n");
    printf("0) Exit\n");
}

/* Read integer helper */
int read_int(const char *prompt, int *out) {
    int v;
    printf("%s", prompt);
    if (scanf("%d", &v) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');
    *out = v;
    return 1;
}

/* Read double helper */
int read_double(const char *prompt, double *out) {
    double v;
    printf("%s", prompt);
    if (scanf("%lf", &v) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');
    *out = v;
    return 1;
}

/* Compute EMI using standard formula */
double compute_emi(double principal, double annual_rate_percent, int months) {
    double monthly = annual_rate_percent / 100.0 / 12.0;
    if (monthly == 0.0) return principal / (double)months;
    double r = monthly;
    double powv = pow(1.0 + r, months);
    double emi = principal * r * powv / (powv - 1.0);
    return emi;
}

/* Show clear loan summary */
void show_summary(double principal, double annual_rate_percent, int months) {
    double emi = compute_emi(principal, annual_rate_percent, months);
    double total = emi * months;
    double interest = total - principal;
    printf("\n--- Loan Summary ---\n");
    printf("Principal     : %.2f\n", principal);
    printf("Annual Rate %% : %.2f\n", annual_rate_percent);
    printf("Tenure months : %d\n", months);
    printf("Monthly EMI   : %.2f\n", emi);
    printf("Total payment : %.2f\n", total);
    printf("Total interest: %.2f\n", interest);
}

/* Show a short amortization preview (first 12 months) */
void show_preview(double principal, double annual_rate_percent, int months) {
    double balance = principal;
    double monthly = annual_rate_percent / 100.0 / 12.0;
    double emi = compute_emi(principal, annual_rate_percent, months);
    printf("\n--- Amortization Preview ---\n");
    printf("Month | EMI   | Interest | Principal | Balance\n");
    printf("------------------------------------------------\n");
    int limit = months < 12 ? months : 12;
    for (int m = 1; m <= limit; ++m) {
        double interest = balance * monthly;
        double principal_paid = emi - interest;
        if (principal_paid > balance) principal_paid = balance;
        balance -= principal_paid;
        if (balance < 1e-8) balance = 0.0;
        printf("%5d | %5.2f | %8.2f | %9.2f | %8.2f\n", m, emi, interest, principal_paid, balance);
    }
    if (months > 12) printf("... (truncated)\n");
}

/* Ask a yes/no question (returns 1 for yes) */
int yes_no(const char *prompt) {
    char line[16];
    printf("%s", prompt);
    if (!fgets(line, sizeof(line), stdin)) return 0;
    return (line[0] == 'y' || line[0] == 'Y');
}

/* Save the full amortization schedule to CSV */
void save_schedule(double principal, double annual_rate_percent, int months, const char *fname) {
    FILE *f = fopen(fname, "w");
    if (!f) { perror("open"); return; }
    double balance = principal;
    double monthly = annual_rate_percent / 100.0 / 12.0;
    double emi = compute_emi(principal, annual_rate_percent, months);
    fprintf(f, "Month,EMI,Interest,Principal,Balance\n");
    for (int m = 1; m <= months; ++m) {
        double interest = balance * monthly;
        double principal_paid = emi - interest;
        if (m == months) { principal_paid = balance; emi = interest + principal_paid; }
        balance -= principal_paid;
        if (balance < 1e-8) balance = 0.0;
        fprintf(f, "%d,%.2f,%.2f,%.2f,%.2f\n", m, emi, interest, principal_paid, balance);
    }
    fclose(f);
}
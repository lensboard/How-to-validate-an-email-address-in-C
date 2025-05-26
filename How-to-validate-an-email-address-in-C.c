#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_EMAIL_LENGTH 256
#define MIN_EMAIL_LENGTH 5  // Minimum realistic email: a@b.c

/**
 * Function: is_valid_email
 * Purpose: Validates an email address according to basic RFC standards
 * 
 * Parameters:
 *   email - pointer to the email string to validate
 * 
 * Returns:
 *   true if email is valid, false otherwise
 * 
 * Validation Rules Applied:
 *   1. Must contain exactly one '@' symbol
 *   2. Must have at least one character before '@' (local part)
 *   3. Must have at least one character after '@' (domain part)
 *   4. Domain must contain at least one '.' after the '@'
 *   5. Must end with at least 2 characters after the last '.'
 *   6. No spaces allowed anywhere in the email
 *   7. Local part cannot start or end with '.'
 *   8. Domain part cannot start or end with '.' or '-'
 */
bool is_valid_email(const char* email) {
    // Check for NULL pointer - safety first
    if (email == NULL) {
        return false;
    }
    
    // Get the length of the email string
    int len = strlen(email);
    
    // Check minimum and maximum length constraints
    if (len < MIN_EMAIL_LENGTH || len > MAX_EMAIL_LENGTH) {
        return false;
    }
    
    // Find the position of the '@' symbol
    // There must be exactly one '@' symbol in a valid email
    int at_pos = -1;
    int at_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            at_pos = i;
            at_count++;
        }
        // Check for spaces (not allowed in email addresses)
        if (isspace(email[i])) {
            return false;
        }
    }
    
    // Must have exactly one '@' symbol
    if (at_count != 1) {
        return false;
    }
    
    // '@' cannot be at the beginning or end
    if (at_pos == 0 || at_pos == len - 1) {
        return false;
    }
    
    // Validate the local part (before '@')
    // Local part cannot be empty and cannot start/end with '.'
    if (email[0] == '.' || email[at_pos - 1] == '.') {
        return false;
    }
    
    // Check for consecutive dots in local part (not allowed)
    for (int i = 0; i < at_pos - 1; i++) {
        if (email[i] == '.' && email[i + 1] == '.') {
            return false;
        }
    }
    
    // Validate the domain part (after '@')
    char* domain = (char*)(email + at_pos + 1);
    int domain_len = len - at_pos - 1;
    
    // Domain cannot start or end with '.' or '-'
    if (domain[0] == '.' || domain[0] == '-' || 
        domain[domain_len - 1] == '.' || domain[domain_len - 1] == '-') {
        return false;
    }
    
    // Find the last dot in domain (for TLD validation)
    int last_dot_pos = -1;
    int dot_count = 0;
    
    for (int i = 0; i < domain_len; i++) {
        if (domain[i] == '.') {
            last_dot_pos = i;
            dot_count++;
        }
    }
    
    // Domain must have at least one dot (for TLD)
    if (dot_count == 0) {
        return false;
    }
    
    // TLD (Top Level Domain) must be at least 2 characters
    if (domain_len - last_dot_pos - 1 < 2) {
        return false;
    }
    
    // Check for consecutive dots in domain (not allowed)
    for (int i = 0; i < domain_len - 1; i++) {
        if (domain[i] == '.' && domain[i + 1] == '.') {
            return false;
        }
    }
    
    // Validate characters in local part
    // Allow alphanumeric, dots, hyphens, underscores, plus signs
    for (int i = 0; i < at_pos; i++) {
        if (!isalnum(email[i]) && email[i] != '.' && 
            email[i] != '-' && email[i] != '_' && email[i] != '+') {
            return false;
        }
    }
    
    // Validate characters in domain part
    // Allow alphanumeric, dots, and hyphens only
    for (int i = 0; i < domain_len; i++) {
        if (!isalnum(domain[i]) && domain[i] != '.' && domain[i] != '-') {
            return false;
        }
    }
    
    // If we've made it through all checks, the email is valid
    return true;
}

/**
 * Function: get_email_input
 * Purpose: Prompts user for email input and validates it
 * 
 * Parameters:
 *   email_buffer - pointer to buffer where valid email will be stored
 *   buffer_size - size of the email_buffer
 * 
 * Returns:
 *   true if valid email was entered and stored, false on error
 * 
 * Features:
 *   - Handles buffer overflow protection
 *   - Clears input buffer after reading
 *   - Provides user feedback on validation errors
 *   - Allows multiple attempts until valid email is entered
 */
bool get_email_input(char* email_buffer, int buffer_size) {
    // Safety check for NULL pointer and valid buffer size
    if (email_buffer == NULL || buffer_size <= 0) {
        printf("Error: Invalid buffer parameters\n");
        return false;
    }
    
    char input_buffer[MAX_EMAIL_LENGTH + 2]; // +2 for newline and null terminator
    
    while (true) {
        // Prompt the user for email input
        printf("Please enter your email address: ");
        fflush(stdout); // Ensure prompt is displayed immediately
        
        // Read input from user
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Error: Failed to read input\n");
            return false;
        }
        
        // Remove trailing newline character if present
        int input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        // Check if input was too long (buffer overflow prevention)
        if (input_len >= MAX_EMAIL_LENGTH) {
            printf("Error: Email address is too long (maximum %d characters)\n", 
                   MAX_EMAIL_LENGTH - 1);
            
            // Clear any remaining input from stdin buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        // Check if user entered empty string
        if (input_len == 0) {
            printf("Error: Please enter a non-empty email address\n");
            continue;
        }
        
        // Validate the email address
        if (is_valid_email(input_buffer)) {
            // Check if the valid email fits in the provided buffer
            if (input_len >= buffer_size) {
                printf("Error: Email too long for provided buffer\n");
                return false;
            }
            
            // Copy the valid email to the output buffer
            strcpy(email_buffer, input_buffer);
            printf("✓ Valid email address entered: %s\n", email_buffer);
            return true;
        } else {
            // Provide specific feedback about why the email is invalid
            printf("✗ Invalid email address. Please check the following:\n");
            printf("  - Must contain exactly one '@' symbol\n");
            printf("  - Must have text before and after '@'\n");
            printf("  - Domain must contain at least one '.' (dot)\n");
            printf("  - Must end with valid domain extension (at least 2 characters)\n");
            printf("  - No spaces allowed\n");
            printf("  - Cannot start or end with '.' or '-'\n");
            printf("  - Length must be between %d and %d characters\n\n", 
                   MIN_EMAIL_LENGTH, MAX_EMAIL_LENGTH - 1);
        }
    }
}

/**
 * Function: main
 * Purpose: Demonstrates the email validation functionality
 * 
 * This example function shows how to use the email input and validation
 * functions in a complete program.
 */
int main() {
    // Buffer to store the validated email address
    char user_email[MAX_EMAIL_LENGTH];
    
    printf("=== Email Address Validation Program ===\n");
    printf("This program will validate your email address format.\n\n");
    
    // Get and validate email input from user
    if (get_email_input(user_email, sizeof(user_email))) {
        printf("\nSuccess! Your email '%s' has been validated and stored.\n", user_email);
        
        // Here you could use the validated email for further processing
        // For example: send confirmation email, store in database, etc.
        
    } else {
        printf("\nProgram terminated due to input error.\n");
        return 1;
    }
    
    return 0;
}

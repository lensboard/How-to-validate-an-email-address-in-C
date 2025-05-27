Main Functions:

is_valid_email() - Performs thorough email validation with multiple checks

get_email_input() - Handles user input with validation and error feedback

main() - Demonstrates usage of the functions

Validation Rules Implemented:

Exactly one '@' symbol required
Non-empty local and domain parts
Domain must contain at least one dot
Top-level domain must be at least 2 characters
No spaces allowed anywhere
No consecutive dots
Proper character validation for both local and domain parts
Length constraints (5-255 characters)

Safety Features:

Buffer overflow protection
Input sanitization
NULL pointer checks
Comprehensive error messages
Multiple input attempts until valid email is entered

User Experience:

Clear prompts and feedback
Detailed error messages explaining validation failures
Visual confirmation with ✓ and ✗ symbols

The code follows best practices with detailed comments explaining the purpose, parameters, return values, and logic of each function. You can compile and run this program to test email validation interactively.

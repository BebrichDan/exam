#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 15
#define MAX_PHONE_LENGTH 11
#define MAX_ADDRESS_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
} Contact;

void createSampleContacts(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Unable to open file");

        return;
    }

    Contact contacts[] = {
            {"Anderson", "1234567890", "123 Elm Street"},
            {"Brown",    "2345678901", "456 Oak Street"},
            {"Davis",    "3456789012", "789 Pine Street"},
            {"Harris",   "4567890123", "101 Maple Street"},
            {"Johnson",  "5678901234", "202 Birch Street"},
            {"Martin",   "6789012345", "303 Cedar Street"},
            {"Taylor",   "7890123456", "404 Walnut Street"},
            {"Wilson",   "8901234567", "505 Ash Street"}
    };

    size_t numContacts = sizeof(contacts) / sizeof(Contact);

    for (int i = 0; i<numContacts; i++)
    {
        fwrite(&contacts[i], sizeof(Contact), 1, file);
    }

    fclose(file);
}

void insertContact(const char *filename, Contact newContact)
{
    FILE *file = fopen(filename, "rb+");
    if (!file)
    {
        perror("Unable to open file");

        return;
    }

    FILE *tempFile = fopen("temp.bin", "wb+");
    if (!tempFile)
    {
        perror("Unable to open temporary file");
        fclose(file);

        return;
    }

    Contact contact;
    int inserted = 0;

    while (fread(&contact, sizeof(Contact), 1, file) == 1)
    {
        if (!inserted && strcmp(newContact.name, contact.name) < 0)
        {
            fwrite(&newContact, sizeof(Contact), 1, tempFile);
            inserted = 1;
        }
        fwrite(&contact, sizeof(Contact), 1, tempFile);
    }

    if (!inserted)
    {
        fwrite(&newContact, sizeof(Contact), 1, tempFile);
    }

    fclose(file);
    fclose(tempFile);


    remove(filename);                                                //замена оригинального файла на временный
    rename("temp.bin", filename);
}

void printContacts(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        perror("Unable to open file");

        return;
    }

    Contact contact;
    while (fread(&contact, sizeof(Contact), 1, file) == 1)
    {
        printf("Name: %s, Phone: %s, Address: %s\n", contact.name, contact.phone, contact.address);
    }

    fclose(file);
}

int main()
{
    const char *filename = "contacts.bin";

    createSampleContacts(filename);                                                           //создание файла с контактами

    Contact newContact = {"Clark", "9999999999", "606 Willow Street"};  //запись для вставки

    insertContact(filename, newContact);

    printf("Contacts after insertion:\n");
    printContacts(filename);

    return 0;
}
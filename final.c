#include <stdio.h>

struct Product {
    int id;
    char name[50];
    float price;
    int stock;
};

struct Sale {
    int productId;
    int quantity;
};

void inputProductData(struct Product *product) {
    printf("Nama produk: ");
    scanf(" %49[^\n]", product->name);

    printf("Harga produk: ");
    scanf("%f", &product->price);

    printf("Stok produk: ");
    scanf("%d", &product->stock);
}

void addSale(struct Sale *sale, struct Product products[], int numProducts) {
   sale->productId = 0;
   sale->quantity = 0;

   printf("ID Produk: ");
   while (scanf("%d", &sale->productId) != 1 || sale->productId < 1 || sale->productId > numProducts) {
       printf("ID Produk tidak valid.\n");
       printf("ID Produk: ");
       while (getchar() != '\n');
   }

   printf("Jumlah: ");
   while (scanf("%d", &sale->quantity) != 1 || sale->quantity <= 0 || sale->quantity > products[sale->productId - 1].stock) {
       if (sale->quantity <= 0) {
           printf("Jumlah harus lebih besar dari 0.\n");
       } else {
           printf("Stok tidak mencukupi.\n");
       }
       printf("Jumlah: ");
       while (getchar() != '\n');
   }

   products[sale->productId - 1].stock -= sale->quantity;
   printf("Penjualan berhasil ditambahkan.\n");
}

void displayTotalSales(struct Sale sales[], struct Product products[], int numSales) {
    printf("\nTotal Penjualan:\n");

    for (int i = 0; i < numSales; i++) {
        if (sales[i].quantity > 0) {
            int productId = sales[i].productId;
            int quantity = sales[i].quantity;
            float total = quantity * products[productId - 1].price;
            printf("%s: %d x Rp%.2f = Rp%.2f\n", products[productId - 1].name, quantity, products[productId - 1].price, total);
        }
    }
}

float calculateTotalSales(struct Sale sales[], struct Product products[], int numSales)
{
    float totalSales = 0.0;

    for (int i = 0; i < numSales; i++) {
        int productId = sales[i].productId;
        int quantity = sales[i].quantity;
        totalSales += quantity * products[productId - 1].price;
    }

    return totalSales;
}


void displayProducts(struct Product products[], int numProducts) {
    printf("\nDaftar Produk:\n");
    for (int i = 0; i < numProducts; i++) {
        printf("%d. %s (ID: %d)\n", i + 1, products[i].name, products[i].id);
        printf("   Harga: Rp%.2f | Stok: %d\n", products[i].price, products[i].stock);
    }
}

void deleteProduct(struct Product products[], int *numProducts) {
    int productId;
    printf("Masukkan ID produk yang akan dihapus: ");
    scanf("%d", &productId);

    if (productId < 1 || productId > *numProducts) {
        printf("ID Produk tidak valid.\n");
        return;
    }

    for (int i = productId - 1; i < *numProducts - 1; i++) {
        products[i] = products[i + 1];
    }

    (*numProducts)--;
    printf("Produk berhasil dihapus.\n");
}

void deleteSale(struct Sale sales[], int *numSales) {
    int saleId;
    printf("Masukkan ID penjualan yang akan dihapus: ");
    scanf("%d", &saleId);

    if (saleId < 1 || saleId > *numSales) {
        printf("ID Penjualan tidak valid.\n");
        return;
    }

    for (int i = saleId - 1; i < *numSales - 1; i++) {
        sales[i] = sales[i + 1];
    }

    (*numSales)--;
    printf("Penjualan berhasil dihapus.\n");
}

void saveProductsToFile(struct Product products[], int numProducts) {
    FILE *file = fopen("products.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan produk.\n");
        return;
    }

    for (int i = 0; i < numProducts; i++) {
        fprintf(file, "%d;%s;%f;%d\n", products[i].id, products[i].name, products[i].price, products[i].stock);
    }

    fclose(file);
}

void saveSalesToFile(struct Sale sales[], int numSales, float totalSales) {
  FILE *file = fopen("sales.txt", "w");
  if (file == NULL) {
    printf("Gagal membuka file untuk menyimpan penjualan.\n");
    return;
  }

  for (int i = 0; i < numSales; i++) {
    if (sales[i].quantity > 0) {
      fprintf(file, "%d;%d\n", sales[i].productId, sales[i].quantity);
    }
  }

  fprintf(file, "Total Penjualan: Rp%.2f\n", totalSales);

  fclose(file);
}

int loadProductsFromFile(struct Product products[]) {
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file produk.\n");
        return 0;
    }

    int numProducts = 0;
    while (fscanf(file, "%d;%49[^;];%f;%d\n", &products[numProducts].id, products[numProducts].name, &products[numProducts].price, &products[numProducts].stock) == 4) {
        numProducts++;
    }

    fclose(file);
    return numProducts;
}

int loadSalesFromFile(struct Sale sales[]) {
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file penjualan.\n");
        return 0;
    }

    int numSales = 0;
    while (fscanf(file, "%d;%d\n", &sales[numSales].productId, &sales[numSales].quantity) == 2) {
        numSales++;
    }

    fclose(file);
    return numSales;
}

int main() {
    struct Product products[100];
    struct Sale sales[100];
    int numProducts = 0;
    int numSales = 0;

    numProducts = loadProductsFromFile(products);
    numSales = loadSalesFromFile(sales);

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Tambah Produk\n");
        printf("2. Tambah Penjualan\n");
        printf("3. Hapus Produk\n");
        printf("4. Hapus Penjualan\n");
        printf("5. Lihat Produk\n");
        printf("6. Lihat Total Penjualan\n");
        printf("7. Keluar\n");
        printf("Pilihan: ");

        if (scanf("%d", &choice) != 1) {
            printf("Input tidak valid. Masukkan angka.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                if (numProducts < sizeof(products) / sizeof(products[0])) {
                    printf("Masukkan data produk %d:\n", numProducts + 1);
                    products[numProducts].id = numProducts + 1;
                    inputProductData(&products[numProducts]);
                    numProducts++;
                } else {
                    printf("Kapasitas produk penuh.\n");
                }
                break;
            case 2:
                if (numProducts == 0) {
                    printf("Tambahkan produk terlebih dahulu.\n");
                } else {
                    addSale(&sales[numSales], products, numProducts);
                    numSales++;
                }
                break;
            case 3:
                if (numProducts == 0) {
                    printf("Tidak ada produk yang bisa dihapus.\n");
                } else {
                    deleteProduct(products, &numProducts);
                }
                break;
            case 4:
                if (numSales == 0) {
                    printf("Tidak ada penjualan yang bisa dihapus.\n");
                } else {
                    deleteSale(sales, &numSales);
                }
                break;
            case 5:
                if (numProducts == 0) {
                    printf("Tidak ada produk yang tersedia.\n");
                } else {
                    displayProducts(products, numProducts);
                }
                break;
            case 6:
                displayTotalSales(sales, products, numSales);
                printf("Total Penjualan: Rp%.2f\n", calculateTotalSales(sales, products, numSales ));
                break;
            case 7:
                saveProductsToFile(products, numProducts);
                float totalSales = calculateTotalSales(sales, products, numSales);
                saveSalesToFile(sales, numSales, totalSales);
                printf("Program berakhir.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (choice != 7);

    return 0;
}

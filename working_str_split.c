#include <stdio.h>
#include <string.h>

int main()  {
    char s1[100] = "programming/abcd ", s2[] = "/";
    int length, j,len,length1;
    char s3[100],s4[100];
    // store length of s1 in the length variable
    length = 0;
    len = strlen(s1);
    printf("len = %d\n",len);

    if(s1[0] == '/') {
        printf("In the if\n");
        ++length;
    }

    while (s1[length] != '/') {
        printf("s1[%d]--->%c ",length,s1[length]);
         ++length;
    }
    
    printf("\nlength = %d\n",length);
    length1= length;
    strncat(s3,s1,length);
    printf("s3==> %s\n",s3);
    for(j=0;j<=len;j++) {
        s4[j] = s1[length1];
        ++length1;
    }
    printf("s4-->%s\n",s4);
    strncat(s3,s2,length);
    printf("--s3==> %s\n",s3);
    strcat(s3,s4);
    printf("final s3==> %s\n",s3);
    return 0;
}

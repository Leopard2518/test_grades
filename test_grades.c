
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
/*
CとRubyは配列の構造が静的,動的と全く違うから、配列の宣言が面倒になってるけど
Rubyの場合は21行目からのコメントの通り,  array = []  で宣言して順次
[受けたテストの数,テストAの点数,テストBの点数,テストCの点数,...]
( 配列名.push(数値) を使用)
って順番に代入していく
↓
2要素目以降を全部足して1要素目の数字で割る->平均点が出る
↓
基準点と比較
↓
出力

で多分いけるはず
*/


//構造体の宣言
typedef struct{
    int32_t *array; //本体のポインタ
    int32_t capacity; //配列の要素数
    int32_t len; //配列の現在の長さ
} DynamicIntArray;

//可変配列のメモリ確保する関数
DynamicIntArray *
DynamicIntArray_New(void){
    //構造体自信のメモリを確保
    DynamicIntArray *self = calloc(1,sizeof(*self));
    if(!self){
        return NULL;
    }

    //配列のメモリを初期値で確保
    self->capacity = 2; //初期容量として要素数2の配列
    self->array = calloc(self->capacity, sizeof(int));
    if(!self->array){
        free(self);
        return NULL;
    }
    return self;
}

//可変配列のリサイズをする関数
DynamicIntArray *
DynamicIntArray_Resize(DynamicIntArray *self, int32_t capacity){
    int32_t byte = sizeof(int);
    int32_t size = capacity * byte;
    int *tmp = realloc(self->array, size); //メモリの再確保

    self->array = tmp;
    self->capacity = capacity;

    return self;
}

//可変配列へのプッシュを行う関数
DynamicIntArray *
DynamicIntArray_PushBack(DynamicIntArray *self, int32_t elem){
    //プッシュ前にリサイズを判断
    if(self->len >= self->capacity){
        if(!DynamicIntArray_Resize(self, self->capacity *2)){
            return NULL;
        }
    }

    self->array[self->len++] = elem; //要素の追加

    return self;
}

//可変配列のメモリ解放を行う関数
void
DynamicIntArray_Del(DynamicIntArray *self){
    if(!self){
        return;
    }

    free(self->array);
    free(self);
}

//-------------------------以上無視-----------------

int main(){
    //変数の宣言
    int numberOfTest = 0;
    int count = 1;
    int sum = 0;
    int judgement = 0;
    char result;

    //配列の作成
    DynamicIntArray *scores = DynamicIntArray_New();

    scores->array=malloc(1024000);

    //受けたテストの数を把握
    printf("受けたテストの数を入力してください\n");
    scanf("%d",&numberOfTest);

    //入力された数を配列に追加
    DynamicIntArray_PushBack(scores, numberOfTest);

    //それぞれの点数を配列の末尾に追加
    for(int i =scores->len; i > 0; i--){
        int score = 0;
        //C言語はDo-while使ってるけど課題の通りredoを使って!
        do{
        printf("%dつ目のテストの点数を入力してください\n",count);
        scanf("%d",&score);
        if(score > 100) {
            printf("100点以下の点数を入力してください\n");
        }
        }while(score > 100);

        //配列の末尾に追加
        DynamicIntArray_PushBack(scores, score);
        count++;
    }
    
    count = 1;
    for(int i = scores->len - 1; i > 0; i--){
        sum = sum + scores->array[count];
    }

    judgement = sum / scores->array[0];

    if(judgement >= 90){
        result = "秀";
    }else if(judgement >= 80){
        result = "優";
    }else if(judgement >= 70){
        result = "良";
    }else if(judgement >= 60){
        result = "可";
    }else{
        result = "不可";
    }
    
    //結果の表示
    count = 1;
    for(int i = scores->len -1; i > 0; i--){
        printf("%dつ目のテストの平均との差は%d,\n", count, judgement - scores->array[count]);
        count++;
    }
    printf("成績は%sです\n", result);
    return 0;
}
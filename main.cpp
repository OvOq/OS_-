#include"stdio.h"
#include"iostream"

using namespace std;

typedef struct  memory_block { //内存块 
	int start, end, progress, state;
	int last, next;
} mb;

int n = 1; //内存块链 数组 最大使用编号 
mb list[1000]; //内存块链 

void apply(int no, int progress, int size) { //申请内存, no 数组内编号 
	int tmp_s = list[no].end - list[no].start + 1;

	if (tmp_s == size && list[no].state == 0) {
		list[no].state = 1; list[no].progress = progress;
	}
	else {//内存大小 大于  申请大小 
		list[n + 1].start = list[no].start;
		list[n + 1].end = list[no].start + size - 1;
		list[n + 1].state = 1;
		list[n + 1].progress = progress;

		n++;

		list[no].start += size;
		list[n].next = no;
		list[n].last = list[no].last;
		list[list[no].last].next = n;
		list[no].last = n;
	}

	return;
}

void free(int no) { //释放内存， no 数组内编号
	list[no].state = 0; list[no].progress = -1;

	int last = list[no].last, next = list[no].next;
	list[no].last = last; list[no].next = next;

	if ((last != -1) && list[last].state == 0) { //上一个为空 ，合并 
		list[no].start = list[last].start;
		list[no].last = list[last].last;
		list[list[last].last].next = no;
	}

	if ((next != -1) && list[next].state == 0) { // 下一个为空， 合并 
		list[no].end = list[next].end;
		list[no].next = list[next].next;
		list[list[next].next].last = no;
	}
	return;
}

void output() { //输出 
	int no = list[0].next;
	while (no != -1) {
		printf("/%d-%d", list[no].start, list[no].end);
		if (list[no].state == 1)
			printf(".1.%d", list[no].progress);
		else
			printf(".0");
		no = list[no].next;
	}
}

int first(int size) {//首次适应，返回空闲内存块号 
	int no = list[0].next;
	while (no != -1) {
		int tmp = list[no].end - list[no].start + 1;
		if (list[no].state == 0) {
			if (tmp >= size)
				return no;
		}
		no = list[no].next;
	}
	return -1;
}

int best(int size) {//最佳适应，返回空闲内存块号 
	int no = list[0].next;
	int tmp_n = -1;
	int mini = 99999;
	while (no != -1) {
		int tmp = list[no].end - list[no].start + 1;
		if (list[no].state == 0) {
			if (tmp >= size) {
				if (tmp < mini) {
					mini = tmp;
					tmp_n = no;
				}

			}
		}
		no = list[no].next;
	}
	return tmp_n;
}

int worst(int size) {//最坏适应，返回空闲内存块号 
	int no = list[0].next;
	int tmp_n = -1;
	int max = -1;
	while (no != -1) {
		int tmp = list[no].end - list[no].start + 1;
		if (list[no].state == 0) {
			if (tmp >= size) {
				if (tmp > max) {
					max = tmp;
					tmp_n = no;
				}
			}
		}
		no = list[no].next;
	}
	return tmp_n;

}

int main() {

	list[0].state = -1; list[0].progress = -1; //内存块链头 
	list[0].start = -1; list[0].end = -1;
	list[0].last = -1; list[0].next = 1;

	int choice, size_m;
	cin >> choice >> size_m;

	list[1].state = 0; list[1].progress = -1; //添加总内存 
	list[1].start = 0; list[1].end = size_m - 1;
	list[1].last = 0;  list[1].next = -1;

	int  num, progress, op, size;
	while (scanf("%d/%d/%d/%d", &num, &progress, &op, &size) != EOF) {
		if (op == 1) {
			int no = -1;
			switch (choice) { // 空闲块选择方法 
			case 1:no = first(size); break;
			case 2:no = best(size); break;
			case 3:no = worst(size); break;
			}
			if (no != -1) //申请内存 
				apply(no, progress, size);
		}
		else {
			int no = list[0].next;
			while (no != -1) {
				if (list[no].progress == progress) {
					free(no); break; //释放内存 
				}
				no = list[no].next;
			}
		}

		printf("%d", num);
		output(); //输出 
		printf("\n");
	}
}

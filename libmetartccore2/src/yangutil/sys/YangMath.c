/*
 * YangMath.c
 *
 *  Created on: 2021年12月27日
 *      Author: yang
 */

#include <yangutil/sys/YangCMath.h>
#include <yangutil/yangtype.h>
#include <yangutil/sys/YangTime.h>
#include <string.h>

uint64_t yang_random() {
	static bool _random_contexttialized = false;
	if (!_random_contexttialized) {
		_random_contexttialized = true;

		srandom((uint64_t) (yang_update_system_time() | (getpid() << 13)));
	}

	return random();
}
void yang_insertSort(int a[], int n) {
	for (int i = 1; i < n; i++) {
		if (a[i] < a[i - 1]) { //若第i个元素大于i-1元素，直接插入。小于的话，移动有序表后插入
			int j = i - 1;
			int x = a[i]; //复制为哨兵，即存储待排序元素
			a[i] = a[i - 1]; //先后移一个元素
			while (x < a[j]) { //查找在有序表的插入位置
				a[j + 1] = a[j];
				j--; //元素后移
			}
			a[j + 1] = x; //插入到正确位置
		}
		// print(a,n,i); //打印每趟排序的结果
	}
}
int32_t yang_insert_uint16(uint16_t a[], uint16_t value, uint32_t *palen) {
	int32_t alen = *palen;
	if(alen==0){
		a[0]=value;
		*palen=1;
		return Yang_Ok;
	}
	for (int i = 0; i < alen; i++) {
		if (i == alen - 1) {
			if (value == a[i])
				return 1;
			//if(value<a[i]){
			if (yang_rtp_seq_distance(value, a[i])) {
				a[alen] = a[i];
				a[i] = value;
			} else {
				a[alen] = value;
			}
			*palen = alen + 1;
			return Yang_Ok;
		}
		if (a[i] == value || a[i + 1] == value)
			return 1;
		//if(a[i]<value&&a[i+1]>value){
		if (yang_rtp_seq_distance(a[i], value)
				&& yang_rtp_seq_distance(value, a[i + 1])) {
			int left = alen - i;
			memmove((char*) a + (i + 2) * sizeof(uint16_t),
					(char*) a + (i + 1) * sizeof(uint16_t),
					sizeof(uint16_t) * left);
			a[i + 1] = value;
			*palen = alen + 1;
			return Yang_Ok;
		}
	}
	return Yang_Ok;
}

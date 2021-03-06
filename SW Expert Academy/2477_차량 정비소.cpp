#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <queue>
using namespace std;

//창구 정보를 저장하는 클래스
class desk {
public:
		bool use;          //현재 사용여부
		int usingPerson;   //현재 사용하고 있는 사람 번호
		int usingTime;     //현재까지 사용한 시간
		//클래스 초기화
		desk() {
			use = false;
			usingPerson = 0;
			usingTime = 0;
		}
};

int main(void)
{	
	int tc; scanf("%d", &tc);
	for (int t = 1; t <= tc; ++t) {
		int n, m, k, a, b;
		scanf("%d %d %d %d %d", &n, &m, &k, &a, &b);
		//1번 인덱스부터 저장
		int at[10], bt[10];
		for (int i = 0; i < n; ++i)
			scanf("%d", &at[i + 1]);
		for (int i = 0; i < m; ++i)
			scanf("%d", &bt[i + 1]);
		int kt[1001];
		for (int i = 0; i < k; ++i)
			scanf("%d", &kt[i + 1]);

		//현재 시간
		int cur_time = 0;
		//서비스를 모두 받은 사람 수
		int servicedPerson = 0;
		//각 창구 정보를 저장하는 벡터(0번 인덱스부터 시작)
		vector<desk> aDesk(n);
		vector<desk> bDesk(m);
		//aq: 접수 창구를 기다리고 있는 대기열
		//bq: 정비 창구를 기다리고 있는 대기열
		queue<int> aq, bq;
		//각각의 사람이 몇번 접수 창구(0) 몇번 정비 창구(1)를 사용했는지 저장하는 배열
		int usedDesk[2][1001];
		//시뮬레이션 시작
		while (true) {
			//기저 사례: 모든 사람이 서비스를 받았다면 반복문을 종료한다.
			if (servicedPerson == k) break;
			//현재 시간에서 정비소에 온 사람을 접수 창구 대기열에 추가한다.
			for (int i = 0; i < k; ++i)
				if (kt[i + 1] == cur_time)
					aq.push(i + 1);

			//접수 창구
			for (int i = 0; i < n; ++i) {
				//현재 접수 창구를 사용하지 않고 있으며, 대기열에 사람이 있는 경우
				if (!aDesk[i].use && !aq.empty()) {
					aDesk[i].use = true;
					int nextPerson = aq.front();
					aq.pop();
					usedDesk[0][nextPerson] = i + 1;
					aDesk[i].usingPerson = nextPerson;
				}
				//현재 접수 창구를 사용하고 있고, 서비스 시간이 끝난 경우
				else if (aDesk[i].use && aDesk[i].usingTime == at[i + 1]) {
					//정비 창구 대기열에 추가해준다.
					bq.push(aDesk[i].usingPerson);
					//접수 창구 대기열에 사람이 있다면 추가한다.
					if (!aq.empty()) {
						int nextPerson = aq.front();
						aq.pop();
						usedDesk[0][nextPerson] = i + 1;
						aDesk[i].usingTime = 0;
						aDesk[i].usingPerson = nextPerson;
					}
					//대기열이 없다면 초기화한다.
					else {
						aDesk[i].use = false;
						aDesk[i].usingPerson = 0;
						aDesk[i].usingTime = 0;
					}
				}
			}

			//정비 창구
			for (int i = 0; i < m; ++i) {
				//현재 정비 창구를 사용하지 않고 있으며, 대기열에 사람이 있는 경우
				if (!bDesk[i].use && !bq.empty()) {
					bDesk[i].use = true;
					int nextPerson = bq.front();
					bq.pop();
					usedDesk[1][nextPerson] = i + 1;
					bDesk[i].usingPerson = nextPerson;
					//정비 창구까지 왔다면 모든 서비스를 받은 것으로 간주한다.
					servicedPerson++;
				}
				//현재 정비 창구를 사용하고 있고, 서비스 시간이 끝난 경우
				else if (bDesk[i].use && bDesk[i].usingTime == bt[i + 1]) {
					//정비 창구 대기열에 사람이 있다면 추가한다.
					if (!bq.empty()) {
						int nextPerson = bq.front();
						bq.pop();
						usedDesk[1][nextPerson] = i + 1;
						bDesk[i].usingTime = 0;
						bDesk[i].usingPerson = nextPerson;
						//정비 창구까지 왔다면 모든 서비스를 받은 것으로 간주한다.
						servicedPerson++;
					}
					//대기열이 없다면 초기화한다.
					else {
						bDesk[i].use = false;
						bDesk[i].usingPerson = 0;
						bDesk[i].usingTime = 0;
					}
				}
			}

			//현재 시간 및 창구에 서비스를 받고 있는 시간을 증가시킨다.
			cur_time++;
			for (int i = 0; i < n; ++i)
				if (aDesk[i].use)
					aDesk[i].usingTime++;
			for (int i = 0; i < m; ++i)
				if (bDesk[i].use)
					bDesk[i].usingTime++;
		}

		int ans = 0;
		for (int i = 0; i < k; ++i) {
			if (usedDesk[0][i + 1] == a && usedDesk[1][i + 1] == b)
				ans += (i + 1);
		}

		printf("#%d %d\n", t, ans == 0 ? -1 : ans);
	}

	return 0;
}

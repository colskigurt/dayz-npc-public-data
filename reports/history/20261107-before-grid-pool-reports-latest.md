# 64개체 밀집 비용 후보 — 기존 좌표 재분석

## 1. 작업 목표
기존 64개체 좌표와 접촉 코드로 밀집 비용 후보를 좁힌다. 새 게임이나 최적화 구현 없이 기존 자료를 활용한다.
## 2. 실제 수행
DGBNativeCrowd.Update/AccumulatePair/ConsumeContact/Displace를 대조했다. 저장된 완전64행 스냅샷에서 floor(x,z) 1m 격자와 인접9칸의 쌍을 재구성했다. 개체 쌍은 한번만 센다. 원본은 해시 전후 일치를 확인했다.
## 3. 측정 결과
새 실행0. 수동 초기130스냅샷의 격자후보 평균636.29/최대1037, 근접쌍 평균213.45. 수동90~100초36스냅샷 후보 평균992.14/최대1689, 근접쌍574.36. 최대 셀 점유는 초기18에서 해당 밀집구간29로 관찰됐다. 평지3실행 재분석 각78스냅샷의 후보평균593.69/575.96/593.04, 최대 셀점유 모두7. 같은64개체의 밀집구간에서 후보평균 약55.9% 증가했으나 CPU 증가율이 아니다. p50/p95/p99 신규 측정 없음. 시각 품질 신규 판정 없음.
## 4. 원인 분석
확인: 공간격자는 이미 구현됐고 PairCandidates/ContactPairs/UpdateTicks도 존재한다. 현재 수동로그에는 이 계수가 없다. 매 Update Cells.Clear 뒤 점유셀마다 새 DGBNativeCrowdCell과 Members 배열을 만든다. 쌍별 힘 계산은 모아두고 ConsumeContact에서 개체별 변위를 검사하므로 쌍마다 전체 SolveContacts를 실행한다는 주장은 틀리다. Displace는 비영 변위에서만 동작한다. 안전검사 뒤 허용된 밀침은 0.5초 제한으로 재계획을 요청한다.
가설: 밀집 후보증가, 셀/문자열 생성, 힘과 변위의 지속으로 늘어나는 후속검사가 비용을 높일 수 있다. 각각 self-time/할당/GC 미측정이다. 스냅샷 좌표는 반올림되고 실제 callback시점과 다르므로 재구성값은 실제 PairCandidates가 아니다. 근접쌍은 movable/combat/entry/force 조건을 반영하지 않아 ContactPairs가 아니다. 개체수의 기하급수 증가로 판정하지 않는다.
반증 경계: 격자가 없거나 모든 쌍에서 전체 지지검사를 수행하는 구조가 아니다. 단순히 이웃 수를 자르면 기존 정상 밀침과 낙하가 바뀔 수 있다.
## 5. 검증 수준
오프라인 구조 진단 성공. 신규 최적화/8·64 회귀/화면/성능 개선/최종수용 미실행. 기존 receiver 방향 생략 후보는 여전히 미수용이다.
## 6. 변경 사항
기존 baseline 연구 폴더에 재구성 Python과 파생 JSON/보고서만 추가. 게임 코드/PBO/서명/실행기/운영 서버 변경 없음. 새 빌드 없음. 원시자료 변경 없음.
## 7. 원시 증거
grid-snapshot-reconstruction.json의 각 source와 SHA256, reconstruct-grid-candidates.py. 기존 contact-context-comparison.json과 DGBNativeCrowd.c를 대조했다. GitHub 참고: recastnavigation/recastnavigation DetourCrowd.cpp의 경로큐 예산·제한 이웃·고정4회 충돌보정, snape/RVO2 Agent.cc의 KD-tree/maxNeighbors, wayne-wu/webgpu-crowd-simulation README의 공간해싱·밀집회피. 외부 코드 성능은 DayZ 개선 증거가 아니다.
## 8. 잔여 문제
다음 단일 산출물은 셀 객체와 격자키 준비의 재사용 후보 타당성표다. 기존 캐시/Scratch 실험과 중복부터 확인한다. 순회순서·각 쌍의 힘·사망 제외·동일프레임 위치를 보존할 수 있어야 한다. 물리검사/이웃 자체를 삭제하지 않는다. 재사용의 관리오버헤드와 실제 전체CPU 효과는 별도 비교가 필요하며 작은 함수 개선만으로 채택하지 않는다. transform batch는 지지비용 해결이 입증되지 않아 자동 구현하지 않는다. B 공개 문서는 이번 오프라인 조사와 기존 실제 비용시험을 구분한다.


### 기존 Scratch와의 중복 및 후보 판정
기존 matched-flat-pursuit 상세 검토와 visible-match direction-review의 Scratch는 SolveContactsInternal 임시 배열이다. 격자 셀 풀은 다른 범위이나 기본 원리는 컨테이너 수명 재사용으로 같다. 둘 다 native Bullet 내부 할당을 제거하지 않는다. 셀 풀의 예상 이점은 생성/해제와 키 준비 감소에 한정되며 실제 절감률 미측정이다. 위험은 이전 Members 잔류, 셀 좌표 혼동, 개체 순서 변경과 메모리 고수위 유지다. 재사용하더라도 매 프레임 이웃 좌표와 힘은 새로 계산해야 한다. 현재는 구현 보류: 먼저 원본과 쌍 목록 및 순서가 동일한지 오프라인 비교하고 생성 수만 산출한다. 그 결과가 좋아도 전체CPU 개선으로 채택하지 않는다.

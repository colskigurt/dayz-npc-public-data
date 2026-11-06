# 현재 진단과 다음 결정

새 게임0. 같은64개체의 초기/밀집구간 격자 후보 평균636/992. 스냅샷 재구성이며 실제 callback 계수·CPU 증가율 아님. 격자 재사용은 타당성 검토,미구현.

[상세](reports/operations/grid-density-existing-evidence-20261107.md) · [근거 TXT](evidence/grid-density-existing-evidence-20261107.txt) · [JSON](api/public/grid-density-existing-evidence.json)

공간격자는 이미 존재하며 쌍마다 전체 SolveContacts를 수행하지 않는다. 힘을 모은 뒤 개체별 변위를 검사한다. PairCandidates/ContactPairs 카운터는 코드에 있지만 현재 수동로그에는 없다. 매Update 셀 재생성은 확인됐으나 할당기/GC self-time 미측정이다. 기존 Scratch는 solver 임시배열이며 셀 풀과 범위가 다르다.

다음은 새 runtime 시험보다 먼저 기존 좌표에서 셀 재사용 시 후보쌍/순서 동등성 확인이다. 이웃제한·지지검사 삭제·낙하복구 변경 없음. 최적화 채택 근거가 아니며 기존 receiver 후보는 미수용. 전체20ms 목표 달성 아님.

# DayZ NPC 현재 상태

최신: [SupportFast64 비용 중간 보고서](reports/operations/supportfast64-cost-interim-20261107.md).

| 구분 | 현재 판정 |
|---|---|
|8개체 기능|8/8, 낙하9/복귀9; 별도 실행 통과|
|64비용 수집|계획6회 중4회 수집,3회 구간 검증,1회 활성 조건 제외|
|완전 비교|1쌍; 광선/게임초58.23% 감소, 서버CPU/게임초5.19% 증가, Present p95 7.12% 증가|
|외부 부하|다른 게임이 이후 관찰됨; 이전 측정과 중첩 여부 미확인|
|최종 판정|성능 개선 미입증·미수용; 전체3쌍 미완료|

추가 게임 실행은 전용 측정 시간 확보 전 보류하고 자료 분석을 진행한다. 광선4회 대신 추가 SphereCast1회와 경계 검사가 있어 광선 수 감소를 총 비용 감소로 해석하지 않는다.

[공개 JSON](api/public/supportfast64-cost-interim-20261107.json) · [이전8개체 결과](reports/operations/supportfast-current-engine-eight-20261107.md) · [수신기 자산 경량화](reports/operations/receiver-native-assets-cost64-three-pairs-20261107.md)

64고저차 기능·시각 품질·전체20ms 목표는 미완료. 정상 밀침·낙하·착지·FOLLOW·재등반 유지.

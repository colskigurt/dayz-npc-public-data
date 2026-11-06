# DayZ NPC 현재 상태

최신 작업: 기존 SupportFast 후보의 현행 엔진8개체 기능 회귀 완료.

| 판정 | 결과 |
|---|---|
|8개체 기능|최종8/8, 전체 경유8/8, 낙하9/복귀9|
|검사량|Fast17,528회, 광선70,112회 생략|
|CPU·Present|이번 실행 미측정; 개선 미입증|
|시각 품질|이번 실행 미평가|
|정상 서버 종료|실제 종료-1로 미수용|
|전체20ms 목표|미달성/이번 실행 미측정|

[최신 상세 보고서](operations/supportfast-current-engine-eight-20261107.md) · [공개 JSON](../api/public/supportfast-current-engine-eight-20261107.json)

이전 수신기 자산 경량화3쌍에서는 클라이언트 Present/CPU 개선을 관찰했지만 서버 CPU 기준은 실패했다. 이번 SupportFast와 결합하지 않았다.

다음: 원본 수신기를 유지한64개체 단일 요인 CPU/Present 반복 비교. 정상 낙하·착지·재등반을 보존한다.

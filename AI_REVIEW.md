# DayZ NPC 현재 상태

최신: [접촉 내부 시간 계측 생략 후보 준비](reports/operations/contact-timing-candidate-prepared-20261107.md).

| 구분 | 판정 |
|---|---|
|변경|기존 물리질의·판정을 보존하고 상세TickCount/누적만 선택OFF|
|근거|기존 기준구간에서 해당계측 약40,222호출/게임초;CPU시간 아님|
|준비|빌드·서명검증·packed소스 일치 완료|
|실행|Enforce 컴파일·8/64기능·CPU/Present 미실행|
|채택|기본OFF·성능미입증·최종미수용|

전용 측정 시간 확보 전 새 게임 실행 보류. 기존 SupportFast의광선감소/CPU미개선·외부부하미통제 기록은 보존한다. 새 후보와 혼합하지 않는다.

[공개 JSON](api/public/contact-timing-candidate-prepared-20261107.json) · [검토용 후보 코드](review/contact-timing-candidate/NFContactTimers.c) · [이전 검사량 분석](reports/operations/supportfast-query-accounting-review-20261107.md)

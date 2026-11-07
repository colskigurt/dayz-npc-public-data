# DayZ NPC 현재 상태

최신: [접촉 계측OFF8개체 비교](reports/operations/contact-timing-eight-pair1-20261107.md).

| 항목 | 결과 |
|---|---|
|후보 기능|8/8 전체경유·최종,낙하9/복귀9,상태오류0|
|계측OFF|8개체 시간0/Unavailable;실제물리호출양수·ray=10×solve|
|대조|300초HIGH횟수조건미완료;그대로보존|
|성능·시각|미측정;기능개선·CPU절감 주장하지않음|
|64비용|6실행3쌍 준비만완료;원본384호스트·FastOFF|
|최종판정|후보단일기능통과,완전쌍회귀/성능/최종수용아님|

기존정체를수정하는기능개발로전환하지않는다. 전용부하조건확보후외부CPU/Present로비교한다. 시간필드0은비용0이아니다.

[공개 JSON](api/public/contact-timing-eight-pair1-20261107.json) · [후보 코드](review/contact-timing-candidate/NFContactTimers.c) · [컴파일 확인](reports/operations/contact-timing-compile-only-20261107.md)

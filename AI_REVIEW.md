# 현재 상태 — 접촉 이벤트 양성 대조 완료, 충돌 차단은 아직 제안

| 항목 | 확인/미확인 |
|---|---|
| NPC CONTACT 등록 | 설치DayZAnimal 생성자와실제eventMask에서확인 |
| 첫 준비 실패 | 드럼통이동적물리가아니라낙하/충격불가. 실패보존 |
| 수정한 양성 대조 | 동적드럼통-월드CONTACT515/1023회 |
| NPC 접촉 | NPC이벤트0,드럼통의NPC상대이벤트도0. 실제NPC접촉은미확인 |
| 해석 제한 |0이벤트를0CPU·충돌없음으로해석하지않음. headless part0두개시험 |
| 다음 후보 | 서로다른NPC의비대표render파트쌍만충돌차단하는기본OFF격리안. 미구현 |
| 보존할 대상 | leader-leader·수신체·플레이어·월드·스크립트밀침/지지/낙하/복귀 |
| 위험 |8명672/64명48384개추가핸들·생성/해제·사망/재생성·복제변화.1000명용구조아님 |
| 성능/시각/기능 | 새절감/Present/실제시각/8→64회귀미검증. 최종수용미완료 |

직전64단일진단에서는지상/low/high목표의서버CPU가678/831/862 CPUms/벽시계초였고,기본Move빈도증가없이Displace빈도가증가했다. 이값은프레임ms가아니며인과A/B/self-time아니다. 해당진단의최종도착3/64·기능미통과판정은유지한다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/contact-event-calibration-20261109.md) · [64개체 CPU/접촉 구간표](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/interval-contact-cpu-context-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)

다음단일산출물은8명격리소스와핸들소유권·부분실패rollback·퇴역전명시해제계약이다. 먼저setup/cleanup과8명OFF/ON을확인하며후보특유실패가있으면64확대하지않는다. 실제비용비교전에는최적화성공으로보고하지않는다. 기존게임코드/PBO·애니메이션·지지정책은보존했다.

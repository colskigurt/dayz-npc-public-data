# DayZ NPC 현재 상태

기존 5파트 플레이 빌드 유지. 경계 재사용은 메서드 진단만 성공, 실제 게임/전체 비용 미수용. 과거 순위 조기종료 후보는 전체 비용 세 쌍에서 개선이 재현되지 않아 미수용.

| 항목 | 확인 범위 |
|---|---|
| 애니메이션 | 사용자 14클립·8개체 시각 평가 성공,64 전체 최종 수용 별도 |
| 최근 64 수동 시험 | 전원 생존 30초 Present p95 27.66ms,단일 실행·수집기 종료 판정 미수용 |
| 기능 잔여 | 한 개체 복구 반복,시체 한 개체 착지 대기 보존 |
| 새 경계 재사용 | 35개 정적 물체 2회·2개 1회,미스매치0;전체 비용 미측정 |
| 순위 조기종료 | 과거 비용 3쌍 방향 불일치·미수용 |
| 다음 단일 작업 | 실제 Update/Apply 결과 동등성 |
| 목표 | 64개체 p95 20ms,미달성 |

[상세](reports/operations/bounds-epoch-method-20261107.md) · [근거 TXT](evidence/bounds-epoch-method-20261107.txt) · [JSON](api/public/bounds-epoch-method.json)

[AI 검토](AI_REVIEW.md) · [직전 안내](reports/history/20261107-before-bounds-README.md)

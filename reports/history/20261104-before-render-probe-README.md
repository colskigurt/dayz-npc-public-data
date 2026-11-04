# DayZ NPC 현재 상태와 대량 호드 소스 분석

문서 갱신: 2026-11-04. 최종 수용과 20ms p95 목표는 미달성이다.

최신 작업은 AntifreeZe·Expansion AI 고정 소스의 대량 호드 적용 분석이다. 중앙 판단 예산·사건 wake·활성 개체 수명 관리를 설계 후보로 선정했다. 새 게임 실행·PBO 변경·성능 개선 입증은 없다.

우선 다음 단일 작업은 판단 scheduler의 shadow 계측 명세와 요청/기한/비용 연결이다. 실제 판단 분산은 그 다음에 한 job씩 검증한다. native 명령·물리 dt·등반/낙하/전투는 판단 스케줄과 구분한다. 별도 애니메이션 세션과 기존 CPU 연구는 유지한다.

[최신 작업 상세](reports/operations/npc-horde-ai-source-review-20261104.md) · [복사용 TXT](reports/operations/npc-horde-ai-source-review-20261104.txt) · [AI 검토 안내](AI_REVIEW.md) · [소스 분석 JSON](api/public/npc-horde-ai-source-review.json)

| 현재 확인 범위 | 근거와 한계 |
|---|---|
| 소스 분석 | AntifreeZe 2a805e1 / Expansion experimental 14fd692. 393개 checkout 파일,25개 선정 구간. 전체 Expansion 감사나 실행 검증 아님 |
| 직접 적용 경계 | AF 확률 token은 전역 cap 아님. 높이만으로 도달 불가 판정 금지. Expansion scheduler도 비선점 soft budget |
| 기존 기능 | entry-facing 후보8명8/8·64명 요구 이력53/64·낙하82/82복귀. 조건당 독립1회,나머지11명 종료 FOLLOW. 이번 소스 검토의 신규 결과 아님 |
| 기존 성능 | matched-flat 별도4회 fresh Present p95 65.47–69.56ms,continued70.09–75.30ms. 현재 소스 분석으로 개선된 값 아님 |
| 남은 대량 호드 조건 | 저비용 표현·다층 공유 경로·멀티플레이 interest·상세/간소화 전투 전환 미검증 |

[최근 판정 실험](reports/latest.md) · [현재 상태 JSON](api/public/current-status.json) · [검증 정책](reports/operations/npc-validation-policy-20261103.md)

[직전 상태표](reports/history/20261104-before-horde-ai-review-README.md) · [PZ 연결 복구](reports/operations/npc-pz-bridge-recovery-20261104.md) · [제한·인라인 비용 판정](reports/operations/npc-terrain-scope-cost-20261103.md)

이전 문서는 history와 고정 commit으로 보존한다. 이 저장소는 보고서·선정 근거의 공개 읽기용이며 게임 자산·키·전체 로컬 원시 기록은 포함하지 않는다.


## 현재 최적화 후속

[원본광선루프분리비용비교](reports/operations/npc-terrain-split-loop-cost-20261104.md) · 루프분리2프로세스/384000호출.제외경로3%실패로terrain후보군보류.다음은Drive/Displace커밋계약검토.전체CPU/Present미측정.

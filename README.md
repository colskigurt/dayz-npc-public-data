# DayZ NPC 연구 — 현재 상태 (2026-11-12)

최근 완료는 **8개체 솔버 호출 귀속 진단**입니다. 최적화 성공이 아닙니다.

| 솔버를 호출한 경로 | 호출 비중 | 솔버 내부 경과시간 비중 |
|---|---:|---:|
| 지상 이동 | 34.48% | 34.02% |
| 밀침 | 28.99% | 23.96% |
| 나머지 상태 | 36.53% | 42.02% |

비중은 전체 서버 CPU 비중이 아닙니다. 현재8개체·약29게임초·ON단독1회이며64/200개체로 일반화하지 않습니다. 과거64의 밀침 호출41.7%는 기존 결과로 재사용했습니다.

전원생존·인식후2m이동8/8,90초종료 유효종결5/8. 시각 품질·Present·성능 개선·최종수용은 미완료입니다. 진단 모듈은 운영 채택하지 않습니다.

[최신 상세 보고서](reports/operations/solver-caller-ownership-eight-20261112.md) · [선정 증거](data/solver-caller-ownership-eight-20261112-evidence.txt) · [최신 작업](reports/operations-latest.md) · [공개 상태](api/public/status.json)

이전 [관절 Sweep 단독8 비교](reports/operations/joint-only-eight-diagnostic-20261112.md)는 평균CPU 관찰감소0.815%·꼬리악화로 미수용. [바닐라 활성 작업량 감사](reports/operations/vanilla-custom-workload-audit-20261112.md)와 함께 읽어주세요.

다음: 기존64 호출조건/Combined-Proposal/SupportFast 등과 대조해 안전하게 줄일 수 있는 실제 재진입 조건을 선정합니다. 같은 호출비중을 재측정하는 시험은 반복하지 않습니다.20ms Present p95는 목표입니다.

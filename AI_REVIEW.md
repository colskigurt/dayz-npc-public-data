# DayZ 대량 호드 소스 분석 검토 안내

문서 갱신: 2026-11-04. 이번 작업은 정적 분석이며 새 성능·시각·기능 실험이 아니다.

최신 작업은 AntifreeZe·Expansion AI 고정 소스의 대량 호드 적용 분석이다. 중앙 판단 예산·사건 wake·활성 개체 수명 관리를 설계 후보로 선정했다. 새 게임 실행·PBO 변경·성능 개선 입증은 없다.

우선 다음 단일 작업은 판단 scheduler의 shadow 계측 명세와 요청/기한/비용 연결이다. 실제 판단 분산은 그 다음에 한 job씩 검증한다. native 명령·물리 dt·등반/낙하/전투는 판단 스케줄과 구분한다. 별도 애니메이션 세션과 기존 CPU 연구는 유지한다.

1. [상세 분석](reports/operations/npc-horde-ai-source-review-20261104.md)에서 AF 명령 분기·시간 누적·소유권과 Expansion FTO 판단 scheduler를 확인한다.
2. [선정 근거](evidence/npc-horde-ai-source-review-20261104.txt)는 고정 upstream URL·행 범위·SHA와 자체 코드 발췌다. 전체 최신 PBO/override 스택의 원문이 아니다.
3. [분석 JSON](api/public/npc-horde-ai-source-review.json)과 [현재 상태](api/public/current-status.json)에서 정적 진단과 기존 실험을 구분한다.

확인된 사실: AF는 개체별 확률/TTL이며 중앙 token pool이 없다. Expansion FTO>0은 cursor·lastUpdateTime·호출후비용으로 판단을 분산한다. 별도 thread 실행 코드는 주석 처리돼 있다. 가까운 player가 없는 FTO=2 경로의 250ms는 판단 주기이며 엔진 애니메이션 interval setter가 아니다.

미검증: 비용 절감량,제어 흐름 위험의 실제 재현,8/64회귀,천 개체 표시,멀티플레이,최종 시각 품질. upstream 상수·clone 확보를 수용 결과로 세지 않는다. 실제 실행 PBO와 이번 자체 소스 snapshot의 전체 동일성도 입증하지 않았다.

우선 검토 질문: 필수 물리/행동을 유지하면서 감각/재계획만 분산할 수 있는가; 최대 대기시간과 단일 native 호출 초과를 어떻게 기록할 것인가; 호드별 공정성·사망/삭제 generation 무효화가 충분한가.

직접 재사용 조건: AF는 GPL-3.0-or-later,Expansion 선정 주요 파일은 CC BY-NC-ND 4.0 표시다. 자유로운 코드 혼합/수정본 배포를 가정하지 않는다. 이번 공개본은 upstream 원문을 복제하지 않고 링크한다.

[기존 상세 상태·후보 판정](reports/history/20261104-before-horde-ai-review-AI_REVIEW.md) · [고정 검증 정책](reports/operations/npc-validation-policy-20261103.md) · [최근 실험](reports/latest.md)


## 현재 최적화 후속

[원본광선루프분리비용비교](reports/operations/npc-terrain-split-loop-cost-20261104.md) · 루프분리2프로세스/384000호출.제외경로3%실패로terrain후보군보류.다음은Drive/Displace커밋계약검토.전체CPU/Present미측정.

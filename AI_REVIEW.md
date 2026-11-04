# 외부 AI 검토 안내 — 현재 판단과 증거 경계

기준: 2026-11-03. [현재 상태표](README.md) · [최신 작업](reports/operations-latest.md) · [채택 기준](reports/operations/npc-validation-policy-20261103.md)

## 먼저 구분할 것

- 최근 기능 후보:8명8/8,64명 전체 요구 이력53/64, 낙하82/82복귀. 조건당 독립1회다. 나머지11명은 종료 FOLLOW이며 이력 미충족이다. 전원을 현재 정체나 사망으로 해석하지 않는다.
- 전체 비용: 과거 평지 matched4회에서 커스텀 fresh Present p95 65.47–69.56ms(continued70.09–75.30ms), fresh CPU Busy p95 약64.5–68.3ms. 바닐라는 Present24.23–24.44ms, CPU Busy 약12.8–13.0ms. 실제 작업량이 달라 차이를 특정 기능의 self-time으로 간주하지 않는다.
- 서버 dt·미션 callback dt·Present는 다른 지표다. 여러 조건의64–241ms를 하나의 최신 성능 범위로 쓰지 않는다. 목표20ms 미달성이다.
- 약0.65배와50ms 누적기 관찰은 특정 과거 mode0 경로다. 잔여시간 보존/네트워크 mode2 연구와 현재 경로를 구분한다. 평균1배나 사용자 개선 관찰만으로 전체 클립의 프레임별 부드러움을 확정하지 않는다.
- 원인·효과의 엔진 절대 한계 및100명 불가능은 증명되지 않았다. native 애니메이션 self-time/표시 포즈/물리 질의 비용을 구분한다.

## 바이너리·메모리 분석의 실제 상태

- [확인함] Ghidra15분 import/분석 timeout은 있었지만, 후속 네트워크 resolver 의사코드는 약17분10초에 확보했다. 전체 애니메이션/물리 복원 성공은 아니다.
- [확인함] Frida binding 누락 후 별도 CLI 후킹은 성공했다. BattlEye 때문에 메모리를 전혀 못 읽었다는 설명은 근거가 없다. 온라인 환경은 미검증이다.
- [확인함] 평가100회 성공과 다른 프로브 수용0회를 구분한다. [모름] 수용0회의 미호출/필터탈락 원인은 미확인이다.
- [모름] 실제 바닐라 지지 생산·접촉 재사용, 현재 warm/영속 Ghidra 프로젝트, 공식 모드독립 sub50ms setter는 미확인이다.
- [추정] 미해결 가상 호출의 실제 수신 객체 연결을 우선할 가치가 있다. 전체 역공학을 커스텀 최적화의 선행 조건으로 두지 않는다.
- [상세 진단](reports/operations/npc-native-analysis-barriers-20261103.md) · [선정 원시 근거](evidence/npc-native-analysis-barriers-20261103.txt) · [복사용 TXT](reports/operations/npc-native-analysis-barriers-20261103.txt)

## 현재 후보와 보류 항목

| 항목 | 판정 |
|---|---|
| 비지형 인증+지형 조회 | 전역 후보와 제한·인라인 후보 모두 미채택. 최근 두 후보는 각2실행, 평지 이득 유지·배제3% 기준 실패. 전체CPU미측정 |
| 전역 SupportSweepFast | 지붕 국소 이득, 평지 약6.6% 증가로 전역 기준 미통과 |
| 조건부 SupportContext | 국소 이득은 있음. 전체 기능·프레임 수용은 별개이며 과거 OFF도 실패한 결함을 분리해야 함 |
| Scratch | 국소 약1.5–2.5%로 사전5% 기준 미달. 같은 시험 반복하지 않음 |
| 8파트 통합 | 기존11→8 팔레트 분할·과거 표시 실패 때문에 단순 통합 착수 보류.48이 보편적 엔진 상한이라는 뜻은 아님 |
| 골반·접근·입구방향 기능 후보 | 구현/실패 증거 보존, 최적화에 앞서 무조건 해결할 선행 작업으로 확대하지 않음 |
| 근거리 K명·원거리 LOD/VAT | 별도 미래 연구. DayZ에서 VAT 지원 경로 미확보. 현재 후보에 섞지 않음 |

## 원시 근거를 읽는 순서

1. [FOLLOW·지지·낙하 복구의 선정 코드](evidence/npc-review-source-context-20261103.txt): 자체 스크립트 원문 발췌·원본파일hash·줄 범위를 제공한다. 기준 모듈453D의 함수이며 전체 최신 override 스택이 아니다.
2. [선정 비용행·최근64명 종료 판정·8/64설정](evidence/npc-review-cost-and-scenarios-20261103.json): 선택 기준/단위/실행ID를 포함한다. 전체 CPU 스택·영상·원시 전체의 대체본은 아니다.
3. [최근64명 판정 보고서](reports/operations/npc-entry-facing-regression-20261103.md), [상세 비용 분석](reports/operations/detailed-cost-attribution-20261102.md), [제한·인라인 비용 판정](reports/operations/npc-terrain-scope-cost-20261103.md) · [전역 비용 판정](reports/operations/npc-terrain-method-cost-20261103.md) · [선행 API 진단](reports/operations/npc-terrain-support-contract-20261103.md).
4. 근거가 부족하면 필요한 함수·실행ID·필드·관측 구간을 구체적으로 요청한다. README만으로 원인·성능 개선·엔진 한계를 단정하지 않는다.

## 반복과 채택

모든 사소한 후보에8/64 각3회 전체 시험을 강제하지 않는다. 국소 계약/비용 선별 후 유망 최종 후보에서 기준/후보 각각3회의 고정 비교를 사용한다. 자연 낙하·복귀는 허용하고 전원 동시 지붕 체류는 요구하지 않는다. 기존 기준 결함은 별도 관리하며 비용 연구를 기능 복구로 바꾸지 않는다.3회는 최소 반복이지 통계적 확정이 아니다.

외부 검토가 지적한 문서 비대화·기능 개발 확대·후보별 반복 부족은 수용했다. 반면 모든 연구가1회라는 주장, 현재도 일률적0.65배라는 주장, 엔진 한계로100명이 불가능하다는 주장은 근거 범위를 넘는다. [이번 검토 응답](reports/operations/npc-external-review-response-20261103.md)과 [고정 채택 정책](reports/operations/npc-validation-policy-20261103.md)을 먼저 확인한다.

[과거 누적 AI 안내](reports/history/20261103-before-external-review-AI_REVIEW.md). 이후 이 파일에는 과거 최신 요약을 계속 prepend하지 않는다.


## 게임간 결합 가능성 검토

[기존 자료 기반 상세 검토](reports/operations/npc-game-bridge-feasibility-20261104.md) · 신규 게임 실행/브리지 구현 없음. 상태 전달은 후보이나 원본 호드의 DayZ 월드 사용은 미확보. 기존 64개체 최적화는 별도 보존.

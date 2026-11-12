# Lazy eligibility — 제외 관측 수 정정·재측정 준비

## 1. 작업 목표
이번 갱신은 표기 정정·재측정 준비 기록이며 새로운 NPC 실행이나 성능 반복이 아니다. 기존 실험 판정은 그대로 보존한다.
동일 lazy eligibility 후보의64개체 전체 CPU 관측을 독립 반복한다. 첫 OFF→ON, 두 번째 ON→OFF, 세 번째 OFF→ON. 코드를 바꾸지 않고 기존3쌍 모두 CPU 감소/중앙5% 및 기능·Present·시각 독립 판정 원칙을 유지한다.

## 2. 실제 수행
정정: 원시 foreground-exclusion.json에는 처음부터26/28이 기록돼 있었으나 앞선 서술에25/28로 옮겨 적었다. 원시 기록과 이전 보고서는 보존하고 이 문서로 정정한다. 세 번째 쌍 제외·미수용 판정은 달라지지 않는다.
첫 비교 LAZY-ELIGIBILITY-cost64-1을 재사용하고 새2쌍을 같은 init.c SHA02782be7…로 실행했다. 원래13개 PBO,64render+64receiver,90게임초,같은 카메라·목표 이동·날씨·40초 워밍업 후30벽시계초 CPU 표본. shadow 재계산은 끄고 양쪽 같은 aggregate 진단을 유지했다. 새 PBO·영상·ETL·덤프 없음.
두 번째 쌍 종료 직후 terminal-only 분석기가 비어 있지 않은 프로세스 inventory로 분석을 거부해 세 번째 실행을 보류했다. 이후 실제 inventory 부재·종료 receipt를 대조하고 같은 원시 자료만 재분석했다. 최초 잔존 프로세스 신원은 저장되지 않아 원인 확정은 불가하다. 이 재분석을 독립 반복으로 세지 않았다. 실패·stop flag를 보존하고 analysis-reconciled.json에 따라 아직 실행하지 않은 세 번째 쌍만 이어갔다.

## 3. 측정 결과
추가 게임 측정0회. 계측 제어의 합성9입력·PowerShell 문법 검사0오류를 확인했다. 이 검사는 상태 분류만 검증하며 실제 프로세스 신원 검사·graceful stop의 런타임 검증은 아니다.
| 비교 | 순서 | 기준 CPU 평균 | 후보 CPU 평균 | 변화 | 조건 |
|---|---|---:|---:|---:|---|
|1|OFF→ON|625.4568|594.0901|-5.0150%|유효|
|2|ON→OFF|612.0723|602.6553|-1.5385%|유효|
|3|OFF→ON|633.6499|591.1894|-6.7009%|백그라운드 전환으로 제외|
CPU 단위는 ms/벽시계초, 프레임 시간이 아니다. 유효 독립2쌍·제외1쌍. 유효 두 값의 중앙 -3.2768%는2쌍 요약일 뿐 최종 효과 추정이나3쌍 기준 통과가 아니다. 첫5.015%만 대표값으로 삼지 않는다.
두 번째 쌍의 CPU rate p50/p95/p99는 기준624.126/747.019/772.936, 후보611.406/716.979/723.599ms/벽시계초. 같은 구간 미션 dt p50/p95/p99는 기준22/32/39, 후보22/32/36ms이며 Present가 아니다. 발 Ray/게임초 기준24762.826/후보24525.979, 관절 Sweep9861.450/9780.030, 군집 inclusive 경과/벽시계 비중7.3997%/6.3905%. 각 물리·궤적 차이는 보존하며 순수 self-time으로 해석하지 않는다.
두 번째 최종 결과 기준 valid15/incomplete47/fall2, 후보 valid11/incomplete50/state2/fall1. 모든 관측 실행에서64개체 생존·인식·2m 이동·정상 종료·native crash 없음.64개체 전체 기능 수용이 아니다.
세 번째 기준에서 클라이언트 비활성992프레임, client age53.5579–105.324. CPU 수집 안의 상태 관측28회 중26회 비활성. 실제 측정 구간이 오염됐으므로 전체 쌍을 제외했다. 누가 포커스를 바꿨는지는 미확인이다. 수집 뒤 불리한 구간을 잘라내거나 세 번째 결과를 유효 반복으로 재분류하지 않는다.

## 4. 원인 분석
확인: 유효한 두 쌍 모두 CPU가 낮게 관측됐지만 -5.02%와 -1.54%로 편차가 있다. 같은 코드·원래 물리 검사이며 전체 개선은 아직 재현 완료가 아니다. 세 번째 수치는 foreground 조건 실패로 사용할 수 없다.
추정: 원래 상태 조회를 줄이는 것이 Crowd.Update 국소 시간 감소에 기여할 수 있다. 전체 CPU 차이는 실제 경로·접촉·FALL 체류·스케줄 차이를 포함해 순수 캐시 효과로 확정할 수 없다.
미확인:3번째 유효 비교, 통계적 확실성, full64 기능 회귀, 실제 시각 품질·Present·GPU·GC. 기존 LANDING_SPACE_BLOCKED/CONTACT_SUPPORT_LOST는 기준에도 있다. 이번 연구를 착지 기능 개발로 전환하지 않는다. 셀 풀링·전체 필드 snapshot·pair-radius의 기존 부족한 결과도 재사용해 중복 후보를 섞지 않았다.

## 5. 검증 수준
반복 진단 부분 성공·조건 불일치로 최종 판단 보류(INCONCLUSIVE_CONDITION_GATE). 유효3쌍 기준 미충족, 성능 채택 아님. 기능·시각·Present 수용도 미완료.20ms p95는 목표다. 실패한 분석·제외된 시험을 숨기지 않고 각각 다른 종류로 기록한다.

## 6. 변경 사항
replacement1에 기존 init.c를 바이트 그대로 준비했다. 준비된 도우미는 시작 후 비활성/개체 수 불일치/3초 상태 지연 시 원인 receipt를 먼저 보존하고, PID·시작UTC ticks·profile을 대조한 자신의 미션에 stop flag만 요청한다. 강제 foreground·kill·다음 조건 자동 실행을 하지 않는다. 빈/부분 기록은 재읽으며 지속 유효 기록 부재는 별도 구분한다. 실제 이 종료 경로는 아직 미시험이다.
동일 후보 미션을 그대로 복사해 해시 대조. 기존 게임 코드/PBO/모델/운영 서버 변경 없음. 신규 PBO가 없어 새 서명 대상 없음. 실행순서·별도 profile·분석/대기 스크립트만 격리한다. 분석 전 종료 확인을 엄격하게 해 원본을 반복 실행하거나 덮어쓰지 않았다. 최초 fail/control 파일·원시 기록 모두 보존한다.

## 7. 원시 증거
이번 추가 근거는 replacement1의 preparation-proof.json, contract.json, guard-functions.ps1, gate-pair.ps1, run-pair.ps1, guard-test-results.json과 기존 foreground-exclusion.json이다. 새 runs/launch가 없는 준비 상태임을 확인했다.
첫 비교의 contract/analysis/publication receipt, 반복 contract/analysis-pair2.json/analysis-pair3.json/series-analysis.json/foreground-exclusion.json/analysis-reconciled.json. 네 실행의 server/client launch·terminal, capture-trigger.json, server-cpu.csv31행씩, gameplay-eight.tsv, terminal-state.tsv, mixed-hosts.tsv, server-frame.tsv, milestones.tsv, part-command-clock.tsv, visual-app-frames.tsv, client-readiness-observations.jsonl, script 로그. 같은 원시 자료 재분석은 새로운 실행이 아니다. 공개 패킷은 선정·집계·자체 코드·CPU 필드 투영·파일 ID/hash만 제공하며 개인 경로·플레이어 식별자·원시 게임 로그를 제외한다.

## 8. 잔여 문제
사용자 전용 시간 답변은 아직 오지 않았다. 준비 완료를 게임 실행·시험 통과로 표현하지 않는다.
마지막 비교를 다시 하려면 약6분 전용 foreground 시간이 필요하다. 사용자에게 가능 시간을 확인 중이며 답변 전 새 게임을 실행하지 않는다. 확보되면 제외된 세 번째 쌍의 두 조건 모두 같은 설정으로 새 실행하고, 제외 기록을 보존한 채 유효3쌍만 판정한다. 유효 반복이 부족한 현재 상태에서 채택하거나 기준을 완화하지 않는다. 이후 CPU 기준을 통과할 때만 Present·시각·기능의 남은 검증을 진행한다.

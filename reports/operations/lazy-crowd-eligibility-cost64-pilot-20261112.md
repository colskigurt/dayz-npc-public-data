# Lazy eligibility — 64개체 전체 CPU 첫 비교

## 1. 작업 목표
8개체 live shadow 불일치0 이후64개체에서 lazy eligibility의 전체 CPU 효과를 선별한다. 같은90게임초 미션·13개 기존 PBO·single render64+receiver64·기준/후보 동일 진단을 쓰고 원래 판정을 재계산하는 shadow는 끈다. 물리·지지·힘·지속 등반·애니메이션 갱신은 유지한다.

## 2. 실제 수행
LAZY-ELIGIBILITY-cost64-1-baseline → on 순서의 독립1쌍. 같은 init.c에서 nf-lazy-eligibility-on.flag만 후보에 추가했다. 정상권한 Steam 실행, 같은 고정 카메라/날씨/플레이어 ground/low/high 이동. 워밍업40게임초 이후 기존 signed-counter 수집기로30벽시계초 전체 프로세스 CPU를 측정했다. 기존 client per-frame appActive와 관측기64/64·서버 host-ready를 대조했다. 새 PBO/엔진 수정 없음.
기준 준비 뒤 대기 도우미가 관측 파일의 순간 빈 읽기로 종료됐다. 원본 오류를 보존하고 같은 서버·클라이언트를 유지한 채 빈 읽기를 건너뛰는 revision2 도우미를 연결했다. 기준 CPU구간의 coarse 관측25회/후보29회이며 공백은 기존 클라이언트 전체 프레임 활성 기록5261/5264행 모두true로 교차 확인했다. 새 영상·ETL·메모리 덤프를 만들지 않았다.

## 3. 측정 결과
| 지표 | 기준 | 후보 |
|---|---:|---:|
|서버 전체 CPU 평균(ms/벽시계초)|625.4568|594.0901|
|동일 CPU 지표 p50/p95/p99|643.761/744.118/806.510|601.912/708.702/729.306|
|수집 게임초 범위|40.6888–70.6545|41.1135–71.0901|
|수집 벽시계초|30.0031|30.0091|
|미션 OnUpdate dt p50/p95/p99(ms)|23/33/40.20|22/32.05/37.01|
|접촉 solve/게임초|2447.665|2485.539|
|발 Ray/게임초|24476.652|24855.387|
|관절 Sweep/게임초|9762.462|9922.506|
|군집 후보 쌍/Update 표본 평균|645.957|668.044|
|Crowd.Update inclusive 경과/벽시계 비중|7.9640%|6.6833%|
전체 CPU 평균 -5.0150%, 절대 -31.3667ms/벽시계초. 이는 ms/frame이나 Present 지표가 아니다. 단 한쌍이며5% 선별선의 여유는0.0150%p뿐이다. 국소 timer 비중은 CPU self-time이 아니며 전체 비중과 합산하지 않는다.
두 조건 모두64/64 생존·목표 인식·2m 이동, 클라이언트 활성 확인, 컴파일/스크립트 오류·native crash 없음. 최종 결과는 기준 VALID5/INCOMPLETE57/STATE1/FALL1, 후보 VALID12/INCOMPLETE47/STATE3/FALL2. 정상 진행 중90초 종료를 포함하므로64개체 기능 수용이 아니다. 장기 FALL 상태 표본은 기준 최대36.8362초/후보6.7699초였고, 각 조건의5초 초과 두 구간은 종료 전 다른 상태로 복귀했다. 이를 후보의 복구 개선 증거로 삼지 않는다.

## 4. 원인 분석
확인: 발 Ray/게임초는 후보+1.55%, 관절 Sweep/게임초+1.64%로 감소하지 않았다. 군집 후보 쌍도 증가했다. 검사 생략에 의한 개선은 아니다. 캐시 계산은 측정 경계 사이86893회, 갱신1379회였다. 군집 inclusive 경과 비중은 약16.08% 감소했다.
추정: 반복 스크립트 상태 조회 감소가 국소 범위 감소에 기여한다. 그러나 전체 CPU 차이 전부를 이 변경의 순수 효과로 귀속할 수는 없다. 실제 궤적·FALL 체류·접촉·상태 분포가 달랐다. UI/스케줄/할당기 self-time도 분리하지 않았다.
기능: 후보 종료 시 unresolved5개로 기준2개보다 많다. 동일 LANDING_SPACE_BLOCKED/CONTACT_SUPPORT_LOST는 기준에도 있었고 기준의 긴 FALL도 확인했다. 따라서 후보 탓으로 확정하거나 기존 결함을 해결한 것으로 보고하지 않는다. 원시 실패 기록을 보존하며 최적화 작업을 기능 개발로 전환하지 않는다.

## 5. 검증 수준
진단 성공·단일 비용 pilot의 수치 기준만 충족. 반복 검증 완료 아님. Host/foreground gate는 통과했지만 기능 회귀는 미확정, 시각 품질·Present·GPU·GC는 미측정, 최종 미수용. 20ms p95 목표 달성으로 보고하지 않는다.

## 6. 변경 사항
별도 mission/init.c의 NFLazyEligibilityCrowd와 연결, ON flag, 같은 진단용 aggregate timer만 추가했다. baseline도 같은 클래스에서 원래 Update/AccumulatePair를 사용한다. 신규 PBO가 없어 새 서명 대상 없음. 기존13개 PBO/엔진 해시는 실행 전에 대조했다. 계약/미션 SHA는 contract.json, 게임·운영 서버·과거 증거는 보존한다. 도우미 두 버전과 오류도 보존한다.

## 7. 원시 증거
contract-before-mission.json/contract.json, mission/init.c, run-pair.ps1, gate-pair.ps1/revision2와 오류, 양쪽 server/client launch·terminal·start receipts, server-cpu.csv, capture-trigger.json, gameplay-eight.tsv, terminal-state.tsv, mixed-hosts.tsv, milestones.tsv, server-frame.tsv, part-command-clock.tsv, visual-app-frames.tsv, client-readiness-observations.jsonl, script 로그, analysis.json, functional-cutoff-review.json, decision.json. 공개에는 자체 후보 코드·전체 집계·선정 기준·원시 파일 ID와 hash만 포함하고 개인 경로/플레이어 식별자/원시 로그는 제외한다.

## 8. 잔여 문제
다음 단일 산출물은 ON→baseline 역순의 독립 비교1쌍이다. 경계선5.015%를 반복 가능한 개선으로 취급하지 않고 반복·기능 분포를 확인한다. 같은 코드로 모든 쌍 개선/3쌍 중앙5% 등 기존 수용 정책을 적용하고 Present와 실제 시각 품질 검증을 별도로 남긴다. 재현 결과가 부족하면 후보를 채택하지 않는다. 기존 지지/착지 결함은 별도 관리하며 새 기능 개발로 확대하지 않는다.

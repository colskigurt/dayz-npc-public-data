# 잔여시간 보존 인과시험: 약0.66배 재생 원인 확인, 시각·배포 수용 보류

작성: 2026-10-31T11:21:31.677183+09:00

## 1. 작업 목표

기존 고정50ms 평가 뒤 초과 누적시간을0으로 버리는 동작이 느린 재생에 기여하는지 인과적으로 시험한다. CPU 포즈와 실제 화면을 별도로 연결하고, 기존 NPC·정상 낙하·복구·운영 서버를 보존한다. 이전에 반려한 표시 fraction만 변경하는 후보와 upstream 누적기 보존은 구분한다.

## 2. 실제 수행

저장된400개 실제 입력을 float32 모델로 재생해 원래 clock/accumulator와 대조했다. 입력7969.000174ms, 원래103회·5150ms 평가, 잔여 보존160회·7999.87793ms 평가였고 초기/끝 누적기 차이를 포함한 비율은0.999984678이었다. 이는 오프라인 반사실 분석이며 게임 성공은 아니다.

첫8초 baseline은 수치 계약을 확인했으나 영상이 훅 구간 전에 끝났다. 해당 영상은 비교에서 제외했다. 녹화가 실제 armed probe를 기다리도록 고친 뒤30초 baseline과 candidate를 각각 실행했다. 실행은 runtime-20261031-110449 → runtime-20261031-111204 순서이며 조건당1회다. 논리 NPC1명·native8파트·고정 run2·AI/루트 이동 OFF·CPU 조회 ON·VSync ON이다.8개체 이동 시험이 아니다. 미션·PBO·그래픽·capture layout 해시의 일치를 검사했다.

entry-only observer가 native reset 전 total과 reset 후 accumulator/descriptor를 확인했다. 후보는0≤total-primary<primary인 경우에만 실제 accumulator와 보간 descriptor numerator에 잔여값을 썼다. 평가 주기50ms, graph clock, 전환 duration, buffer 포인터, PBO는 바꾸지 않았다.8파트가 모두 모인 경계에서 시작하고 완전한8파트 프레임에서 종료했다. 긴 backlog는 원래 동작으로 복귀하는 guard를 두었다. 메모리 인과시험이며 배포 가능한 스크립트 수정이 아니다.

각45초 창 녹화가 정상 종료됐고 actor/HUD를4시점 이미지로 확인했다. 영상5/12/20초부터 각3초를 분석했다. 실제 QPC origin을 nonce ACK와 대조한 기존 매핑으로 native→다음 CPU 조회 순서를 연결했다. 앞뒤250ms를 제외한 활성 구간에 양 끝이 포함되고 연속 script frame인 영상 쌍만 비교했다.

## 3. 측정 결과

| 항목 | 기본 | 잔여 보존 후보 |
|---|---:|---:|
| native entry / frame |11992 /1499|11992 /1499|
| graph+accumulator / 실제 시간,8파트 중앙값 |0.659440729|1.000003060|
| 독립6관절 source-phase 추정 / mission 시간 |0.657745777|0.999110643|
| 파트당 native 평가 |395|600|
| 숫자 변경 |0|4800|
| 영상 HUD 정확 일치 /540 |536|527|
| 활성·연속 CPU/영상 쌍 |437|431|
| CPU 자세 변화와 윤곽 변화 상관 |0.943663|0.915311|
|6관절 RMS step p50/p95/p99,mm |21.556/117.236/128.025|41.253/121.039/140.869|
| source phase step p50/p95/p99,ms |8.333/37.500/37.500|16.667/37.500/41.667|
| source fit RMS p50/p95,mm |5.580/13.072|8.317/20.889|
|10pixel 미만 body hold / admitted pairs |1/437|0/431|
| 활성 구간 mission pacing p50/p95/p99,ms |20/34/36|20/34/36|

clock 비율 변화는+0.340562331, 상대+51.644%다. 평가 횟수는+51.899% 증가했다. 이는 비용 절감 수치가 아니다. 두 run 모두8파트 clock/numerator spread0, 평가 여부 불일치0, source-phase 음수 step0, guard0이었다. observer/drop 오류0이며 정상 종료했다. observer read bracket p50/p95/p99는 기본0.0103/0.0177/0.0200ms, 후보0.0113/0.0493/0.052309ms이며 함수 실행 시간은 아니다.

## 4. 원인 분석

확인된 사실: 동일 구현에서 잔여시간만 보존하자 clock 진행과 독립 source-pose 진행이 함께 약0.66배에서1배로 바뀌었다. 초과분 reset이 이 조건에서 느린 재생에 기여한다는 인과 증거다. 실제 화면 변화도 CPU 변화와 연관됐다. clock 개선을 보간 전체 또는 최종 GPU 경로 복원으로 확대하지 않는다.

미확인/추정: 불균등한 frame 간격과 이전 frame dt 전달이 남은 step 변동에 기여할 수 있다. 후보의 큰 step과 source fit 오차는 남아 있다. 재생이 빨라지면 자연히 frame당 이동량도 커지므로 step 증가만으로 악화라고 확정하지 않는다. 원본 pose 곡률·표시 지연·시간 정렬을 분리해야 한다.

이번 run에서 지지되지 않은 설명: source-phase가 반복 역행한다는 설명은 음수 step0으로 지지되지 않는다.10pixel hold가 거의 없다는 것만으로 부드러움을 선언할 수도 없다. 후보20초 영상의 거친 counter signature heuristic은 false였지만, 별도 HUD exact matching으로 검증된172/180 frame 중 활성·연속 쌍만 사용했다. 해당 heuristic은 시각 수용 근거로 쓰지 않았다.

## 5. 검증 수준

- 시간 소실 인과시험: 진단 성공,1개 A→B 쌍. 반복 검증 완료 아님.
- CPU/native/영상 연결: 진단 성공. source FK·윤곽은 proxy, 최종 GPU/Present 미측정.
- 배포 가능한 수정·14개 clip·8/64개체 기능 회귀·혼잡/등반 실패 해결·시각 최종 수용·성능 수용: 미완료.
-20ms p95는 유지되는 목표다. 위1개체 observer pacing을64개체 성능으로 해석하지 않는다.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. 새 PBO·재서명·배포 없음. 실행 전 기존3개 PBO 서명 검증 성공. 격리 observer/model/driver30, archive launcher, armed 녹화 gate, replay·시간/pose/영상 비교 도구를 추가했다. 후보 실행에서 native accumulator/descriptor 숫자를 변경했다는 사실을 명시한다. plan의 liveCandidateExecuted=false는 실행 전 계획 상태이며 현재 결과는 comparison/status가 기준이다. 공용 배포본·운영 서버·원본 증거·개인키는 변경하지 않았다.

## 7. 원시 증거

실행 ID는 위 두 runtime과 초기 runtime-20261031-105752다. plan/admission/source snapshots, calls/status/configuration, ACK, CPU TSV, finalized MKV, HUD/윤곽/pose alignment, source FK fit/pairs, comparison/evidence를 기존 폴더에 보존한다. 기존 해시와 파일 안정성을 검증한 뒤 기존 checkpoint 게시 함수를 사용한다. 공개본은 요약·선정 수치·모델 원문이며 영상/전체 로그/주소/프로세스·개인키를 노출하지 않는다. 비교 조건과 source hash를 원본 보고서에 연결한다.

## 8. 잔여 문제

다음은 역순 후보→기본 반복과 frame dt/세대 경계별 phase·화면 속도 분포 비교다. 반복에서 clock 효과가 유지되더라도 실제 부드러움 수용은 별도다. 긴 frame guard가 실제로 발동하는 조건,8파트 동기,CPU 부담을 검사해야 한다. 지원되는 DayZ 스크립트/자산 계약에서 동일 효과를 낼 수 있는지 조사하고, 없다면 배포 한계를 명확히 보고한다. 임의 duration/time 변수 또는 이전에 실패한 fraction-only 후보를 다시 해법으로 쓰지 않는다.

이번 진단 때문에 B8FB 재접근 기능을 완료로 바꾸지 않는다.64/64 유효 종료·원래 actor61 혼잡 재현·등반 종료 계약 및 정상 낙하 회귀와64비용 귀속은 별도 시험으로 계속한다. 자원은 우선 반복 가능한 시각 증거와 지원 구현 경로에 배분하고, 새 군집 설계·대량 자산 추가는 보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `c4a1d50b9cdca93a145ccee459133a449eb64d5b3bd957c9e8de907ac1aa4205`. 원시 전체가 아니다.

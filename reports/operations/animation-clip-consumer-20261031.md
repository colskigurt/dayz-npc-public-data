# 클립 샘플 시간까지 연결: 느린 시계 진행의 하류 확인

작성: 2026-10-31T10:08:55.987348+09:00

## 1. 작업 목표

선택한 NPC root evaluator의 시간 문맥이 실제 클립 처리 직전까지 어떻게 전달되는지 확인한다. CPU 포즈/최종 화면은 별개로 판정한다. 기존 정상 낙하·복구·게임 구현과 운영 서버는 보존한다.

## 2. 실제 수행

원본 EXE SHA-256 c6709ece5468fadeb7eda085355a7dc5157601ddc589fb2beac6f5726a99f933의 bounded Capstone 명령 분석으로 root evaluator → 23종 명령 분기 → opcode 2/3/4 helper → clip kernel 후보를 좁혔다. 호출을 재귀적으로 모두 분석하거나 Ghidra 전체 함수 복원을 완료한 것은 아니다. 미해결 간접 호출은 남아 있다.

기존 B8FB 고정 run2, 논리1개체/네이티브8파트, AI/루트 이동 OFF, VSync ON,120초 실행에 entry-only Frida 관찰8초를 연결했다. root의 실제 caller/vtable/73본 renderer를 검증하고, dispatcher에서 evaluator+0x20 소유와 복사된 oldClock/dt를 재검사했다. kernel에서는 검증된 helper 반환주소와 command+3 opcode를 대조했다. FP 레지스터를 직접 읽은 것이 아니라 정적 movss 출처인 command+8/+12와 리소스+0x64/+0x68을 읽었다. 인자/결과/시간/포즈 override는 없다. 관찰 종료 후 훅을 비활성화하고 게임 정상 종료까지 유지했다.

일반 launcher는 공용 D61D와 씬 요구 B8FB 불일치로 실행 전에 중단됐다. 공용본을 교체하지 않고 별도 launcher에서 기존 B8FB archive를 직접 지정했다. 첫 archive 서명 검사에는 슬래시 경로 형식 오류가 있어 파일을 열지 못했고 실행 전 중단됐다. Windows 경로 형식으로 바로잡은 후3개 의존 PBO 서명 검증이 모두 성공했다. 실패한 사전검사를 게임 실행/회귀 실패로 세지 않는다.

## 3. 측정 결과

- 실행 ID runtime-20261031-100536, 정상 종료, 반복1회. root/dispatcher/kernel 각각 **100/100/100회**, 선택 opcode2. 오류·drop0.
- 리소스 정수+0x64 **60**, float+0x68 **2.083333254**. 변환 manifest run2의 **60.0fps / 126frames / 2.083333333초**와 일치 범위.
- command+12는 **100/100회** root contextDt와 정확히 일치했다.
- command+8의 주기 되감김 **2회**를 길이로 풀었을 때 증가 **4.950018883초**, root oldClock 증가 **4.950018883초**, 인접 증가 최대 잔차 **0초**.
- kernel 호출 간격 p50/p95/p99 **80.0/82.0/82.03999999999999ms**. clip 위치 증가/host 시간 비율 **0.624845**. host는 Date.now 밀리초이며 QPC가 아니다.
-15~55초 mission frame p50/p95/p99 **20.0/35.0/37.0ms**. 훅 포함 진단 실행으로 성능 개선 근거에 사용하지 않는다.

실제 영상·GPU Present·최종 표시 포즈·64개체 기능/비용은 이번에 측정하지 않았다. visualCapture는 HUD 플래그이며 녹화 완료 증거가 아니다.

## 4. 원인 분석

확인된 사실: opcode2 저장 operand command+8은 kernel에서 정수 리소스+0x64와 곱해져 정수 index와 fractional 위치 계산에 사용된다. 하류4aa1f0은 인접 key 데이터와 fractional 가중치로 보간하는 분기를 포함한다. 런타임의60 및2.083333초 주기와 manifest를 대조하면 command+8을 run 클립 샘플 시간(초)로 해석할 근거가 생겼다. 별도 float+12는 동일한 root dt다. 실제 lower decoder 진입/출력은 이번에 훅하지 않았다.

원인 범위를 좁힌 결과: root 입력에서 확인한 느린 시계 진행이 클립 샘플 입력까지 이어진다. 하류에서 시간 부족을 완전히 보충한다는 가설을 이 선택 경로에서는 지지하지 않는다. 다만 최종 GPU 표시에서 얼마나 추가 hold/보간되는지, 느린 재생과 시각적 계단 현상이 얼마나 나뉘는지는 미확인이다. 이 결과만으로 모든 끊김 원인을 완전히 특정하거나60fps 자산이 항상 부드럽다고 주장하지 않는다.

가설/다음 질문: 엔진의 평가 주기·누적기 reset 경로를 보존하면서 충분한 시간을 전달할 수 있는 지원 계약이 있는가? 현재 확보되지 않았다. 새로운 시간 주입·프레임 생략·보간을 적용하지 않았다.

## 5. 검증 수준

선택 클립 샘플 저장 인자의 root 시간 연결: **진단 성공,1회100표본**. lower decoder index/출력·실제 표시 시간 동기화: 미실행. 수정 효과·반복 검증·8/64 회귀·시각 최종 수용·64비용·20ms p95: 미완료. 과거64/64 결과와 이번1개체 진단을 합쳐 전체 수용으로 간주하지 않는다.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. 새 PBO/재서명/배포 없음. 기존 서명3개 검증 성공, 개인키 미포함. 새 entry 관찰기·격리 driver/launcher·bounded static 분석·결과 분석만 추가했다. 공용 D61D와 원시 기록은 변경하지 않았다.

## 7. 원시 증거

plan.json, launch-preflight-block.json, attach-readiness.json, consumer-static*.json과 대응 asm, results.json, pacing-driver.json을 보존했다. raw calls.jsonl/status/configuration/instrumentation/observer driver/launch/manifest 해시를 분석 때 대조하고 보고서 저장 때 다시 확인한다. 공개본은 원시 전체 대신 첫3/마지막3 이벤트와 선택 명령 근거를 제공하며 로컬 경로·프로세스/스레드 ID·실행 포인터를 제외한다.

## 8. 잔여 문제

다음은 클립 시간과 실제 표시 프레임을 동일 실행에서 연결하는 측정, 지원되는 평가/시간 전달 제어의 단위·reset·세대 계약 조사다. lower decoder 입력/결과 관찰이 필요하면 별도 bounded observer로 진행한다. 표시 경로가 계측 불가능하면 그 한계와 필요한 영상/도구를 명시한다.64 동적 혼잡 실패 재현과 성능 귀속은 독립 조건으로 계속 관리한다. 비교 조건이 흐려지는 새 최적화는 보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `12fc84bd2256869da72403ba8f781d0aed22eb164e4428b746e730b7e284dd3d`. 원시 전체가 아니다.
